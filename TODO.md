A list of things that need doing, they don't necessarily need to be done in order.

#### Todo:
- Clean up various hardcoded behaviour (like the player)
- Replace compression system for PAKs with LHA
- Add chunking to PAKs, split it into multiple files over a certain size
- Add type and size exclusions to PAKs (allow for large files such as music to be excluded, but still be loaded in the VFS)
- Add basic encryption feature to the PAK system (to prevent datamining in certain games)
    - Figure out a way to embed the key in the executable?

#### Finish:
- Tilemap collision
- implementing ECS, and convert existing features to use it.
- implementing YAML for existing features

#### Future:
- Tenshi IDE
- Custom build system to make IDE development easier
- Engine extensions/plugins
- IDE extensions/plugins