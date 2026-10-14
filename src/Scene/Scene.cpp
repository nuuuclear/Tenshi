#include "Tenshi/Scene/Scene.h"

#include "Tenshi/Core/Game.h"

#include <algorithm>
#include <stdexcept>

#include "Tenshi/Graphics/Sprite.h"

namespace Tenshi {

Scene::Scene(Game* game) {
	batch = std::make_unique<SpriteBatch>(game->getRenderer());

	camera = std::make_unique<Camera>();

	world.addSystem("character control", [](World& owner) {
		owner.each<CharacterController, Velocity, SpriteRenderer>(
			[](Entity, CharacterController& controller, Velocity& velocity, SpriteRenderer& renderer) {
				velocity.x = controller.walkSpeed * controller.moveAxis;
				velocity.y -= controller.jumpSpeed * (controller.jumpPressed ? 1.0 : 0.0);
				if (controller.moveAxis != 0.0)
					renderer.facingDirection = controller.moveAxis < 0.0 ? -1 : 1;
			}
		);
	}, 0);

	world.addSystem("character physics", [this](World& owner, float dt) {
		owner.each<Transform, Velocity, CharacterController, CharacterCollider>(
			[this, dt](Entity, Transform& transform, Velocity& velocity,
				CharacterController& controller, CharacterCollider& collider) {
				velocity.y = std::min(
					velocity.y + controller.gravity * static_cast<double>(dt),
					controller.terminalVelocity
				);

				const double moveX = velocity.x * static_cast<double>(dt);
				transform.x += moveX;
				Rect_d box = collider.feet.getBounds(transform.x, transform.y);

				for (const Rect_d& obstacle : worldColliders) {
					if (!Collider::intersects(box, obstacle))
						continue;

					transform.x = moveX > 0.0
						? obstacle.x - collider.feet.offsetX - collider.feet.w
						: obstacle.x + obstacle.w - collider.feet.offsetX;
					
					velocity.x = 0.0;
				}

				const double moveY = velocity.y * static_cast<double>(dt);
				transform.y += moveY;
				box = collider.feet.getBounds(transform.x, transform.y);

				for (const Rect_d& obstacle : worldColliders) {
					if (!Collider::intersects(box, obstacle))
						continue;
					
					transform.y = moveY > 0.0
						? obstacle.y - collider.feet.offsetY - collider.feet.h
						: obstacle.y + obstacle.h - collider.feet.offsetY;
					
					velocity.y = 0.0;
				}
			}
		);
	}, 10);

	world.addSystem("character animation", [](World& owner, float dt) {
		owner.each<CharacterController, SpriteRenderer>(
			[dt](Entity, CharacterController& controller, SpriteRenderer& renderer) {
				if (!renderer.sprite)
					return;
				
				renderer.sprite->setBlock(controller.moveAxis == 0.0 ? "idle" : "walk");
				renderer.sprite->step(dt);
			}
		);
	}, 20);

}

Scene::~Scene() {
}

void Scene::init() {
}

void Scene::Step(double dt) {
	world.update(static_cast<float>(dt));

	const auto targets = world.view<CameraTarget>().entities();
	if (!targets.empty()) {
		const Entity target = targets.front();
		const auto* transform = world.tryGet<Transform>(target);
		const auto* velocity = world.tryGet<Velocity>(target);
		const auto* renderer = world.tryGet<SpriteRenderer>(target);

		if (transform && velocity && renderer && renderer->sprite) {
			camera->update(dt,
				transform->x + renderer->sprite->getWidth() / 2.0,
				transform->y + renderer->sprite->getHeight() / 2.0,
				velocity->x, velocity->y);
		} else {
			camera->update(dt);
		}
	} else {
		camera->update(dt);
	}

	if (background)
		background->Step(*camera);
}

void Scene::Draw() {
	if (background)
		background->Draw(*camera);
	if (tilemap)
		tilemap->Draw(*camera);

	batch->begin();

	world.each<Transform, SpriteRenderer>(
		[this](Entity, Transform& transform, SpriteRenderer& renderer) {
			if (!renderer.visible || !renderer.sprite)
				return;

			renderer.sprite->setFlip(renderer.facingDirection < 0
				? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

			renderer.sprite->draw(*batch, *camera,
				static_cast<int>(transform.x), static_cast<int>(transform.y)
			);
		}
	);

	batch->flush();
}

void Scene::setBackground(std::unique_ptr<Background> bg) {
	background = std::move(bg);
}

void Scene::setTileset(std::unique_ptr<Tileset> ts) {
	tileset = std::move(ts);
	if (tilemap) {
		tilemap->SetTileset(tileset.get());
	}
}

void Scene::setTilemap(std::unique_ptr<Tilemap> map) {
	tilemap = std::move(map);
	if (tilemap) {
		tilemap->SetTileset(tileset.get());
	}
}

Entity Scene::createCharacter(Sprite* sprite, double x, double y) {
	if (!sprite)
		throw std::invalid_argument("A character requires a sprite");

	const Entity entity = world.create();
	world.emplace<Transform>(entity, x, y);
	world.emplace<Velocity>(entity);
	world.emplace<CharacterController>(entity);

	CharacterCollider collider;
	collider.feet.w = 6;
	collider.feet.h = 8;
	collider.feet.offsetX = (sprite->getWidth() / 2) - 3;
	collider.feet.offsetY = sprite->getHeight() - 8;
	world.emplace<CharacterCollider>(entity, collider);
	world.emplace<SpriteRenderer>(entity, sprite, true, 1);

	SpriteBlock& idle = sprite->createBlock("idle");
	idle.addFrame(SpriteFrame{{2, 4}, {0.9f, 0.1f}});
	idle.addFrame(SpriteFrame{{3, 5}, {0.9f, 0.1f}});
	idle.setSpeed(3.0f);

	SpriteBlock& walk = sprite->createBlock("walk");
	walk.addFrame(SpriteFrame{{0}, {1.0f}});
	walk.addFrame(SpriteFrame{{2}, {1.0f}});
	walk.addFrame(SpriteFrame{{1}, {1.0f}});
	walk.addFrame(SpriteFrame{{2}, {1.0f}});
	walk.setSpeed(5.0f);
	sprite->setBlock("idle");

	return entity;
}

void Scene::setCameraTarget(Entity entity) {
	if (!world.alive(entity))
		throw std::invalid_argument("Camera target must be a live entity");

	world.each<CameraTarget>([&](Entity target, CameraTarget&) {
		if (target != entity)
			world.remove<CameraTarget>(target);
	});

	if (!world.has<CameraTarget>(entity))
		world.emplace<CameraTarget>(entity);
}

Entity Scene::getCameraTarget() const {
	const auto targets = world.view<CameraTarget>().entities();
	return targets.empty() ? Entity{} : targets.front();
}

World& Scene::getWorld() {
	return world;
}

const World& Scene::getWorld() const {
	return world;
}

std::unique_ptr<Background>& Scene::getBackground() {
	return background;
}

std::unique_ptr<Tileset>& Scene::getTileset() {
	return tileset;
}
std::unique_ptr<Tilemap>& Scene::getTilemap() {
	return tilemap;
}

std::vector<Rect_d>& Scene::getColliders() {
	return worldColliders;
}

std::unique_ptr<Camera>& Scene::getCamera() {
	return camera;
}

} // namespace Tenshi
