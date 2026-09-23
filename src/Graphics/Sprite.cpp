#include "Tenshi/Graphics/Sprite.h"

#include "Tenshi/Scene/Camera.h"

#include <stdexcept>

namespace Tenshi {

SpriteImagePosition SpriteFrame::getRandomImage() const {
    if (images.empty()) return 0;

    if (chance.size() != images.size())
        return images[0];

    float total = 0.0f;
    for (float c : chance)
        total += c;

    float r = ((float)rand() / RAND_MAX) * total;

    float accum = 0.0f;
    for (size_t i = 0; i < images.size(); i++) {
        accum += chance[i];
        if (r <= accum)
            return images[i];
    }

    return images.back();
}

void SpriteBlock::setSpeed(float speed) {
    animationSpeed = speed;
}

void SpriteBlock::setFrame(int f) {
    if (frames.empty()) return;

    frame = f % frames.size();
    timer = 0.0f;

    currentImage = frames[frame].getRandomImage();
}

void SpriteBlock::step(double dt) {
    if (frames.empty()) return;

    if (animationSpeed <= 0.0f) return;

    float frameTime = 1.0f / animationSpeed;

    timer += (float)dt;

    while (timer >= frameTime) {
        timer -= frameTime;

        frame = (frame + 1) % frames.size();
        currentImage = frames[frame].getRandomImage();
    }
}

SpriteImagePosition SpriteBlock::getCurrentImage() const {
    if (frames.empty()) return 0;

    if (frame < 0 || frame >= (int)frames.size())
        return 0;

    return currentImage;
}

void SpriteBlock::addFrame(const SpriteFrame& f) {
    frames.push_back(f);
}

Sprite::Sprite(SDL_Texture* tex, int iW, int iH)
    : texture(tex),
      imageWidth(iW),
      imageHeight(iH)
{
    src = { 0.0f, 0.0f, static_cast<float>(iW), static_cast<float>(iH)};
    dst = { 0.0f, 0.0f, static_cast<float>(iW), static_cast<float>(iH)};

    if (!texture) {
        throw std::runtime_error("Sprite created with null SDL_Texture");
    }

    if (imageWidth <= 0 || imageHeight <= 0) {
        throw std::runtime_error("Sprite created with invalid dimensions");
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

Sprite::~Sprite() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

SpriteBlock& Sprite::createBlock(const std::string& handle) {
    blocks.emplace_back(handle);
    return blocks.back();
}

void Sprite::setImage(int index) {
    if (!texture) return;

    if (imageWidth <= 0 || imageHeight <= 0)
        return;

    if (index < 0) return;

    image = index;

    float texWf, texHf;

    if (!SDL_GetTextureSize(texture, &texWf, &texHf))
        return;

    int texW = static_cast<int>(texWf);

    int columns = texW / imageWidth;
    if (columns <= 0) return;

    int x = index % columns;
    int y = index / columns;

    src.x = static_cast<float>(x * imageWidth);
    src.y = static_cast<float>(y * imageHeight);
}

void Sprite::setOrigin(float x, float y) {
    originX = x;
    originY = y;
}

void Sprite::centreOrigin() {
    originX = imageWidth * 0.5;
    originY = imageHeight * 0.5;
}

void Sprite::setScale(float s) {
    scale = s;
}

void Sprite::setRotation(float r) {
    rotation = r;
}

void Sprite::setFlip(SDL_FlipMode f) {
    flip = f;
}

SDL_Texture* Sprite::GetTexture() {
    return texture;
}

void Sprite::step(double dt) {
    if (currentBlock < 0 || currentBlock >= (int)blocks.size())
        return;

    blocks[currentBlock].step(dt);

    int img = blocks[currentBlock].getCurrentImage();
    setImage(img);
}

void Sprite::resetFrame() {
    if (currentBlock < 0 || currentBlock >= static_cast<int>(blocks.size()))
        return;

    blocks[currentBlock].setFrame(0);
}

void Sprite::setBlock(const std::string& handle) {
    for (size_t i = 0; i < blocks.size(); i++) {
        if (blocks[i].getHandle() == handle) {
            currentBlock = static_cast<int>(i);
            return;
        }
    }
    
    // handle not found
    currentBlock = -1;
}

void Sprite::draw(SpriteBatch& batch, const Camera& cam, int x, int y) {
    SDL_FRect dst;

    int camX = cam.getViewX();
    int camY = cam.getViewY();

    dst.x = x - camX;
    dst.y = y - camY;
    dst.w = imageWidth * scale;
    dst.h = imageHeight * scale;

    SDL_FPoint origin = { originX * scale, originY * scale };

    batch.draw(
        texture,
        src,
        dst,
        rotation,
        origin,
        flip
    );
}

int Sprite::getWidth() {
    return imageWidth;
}

int Sprite::getHeight() {
    return imageHeight;
}

} // namespace Tenshi