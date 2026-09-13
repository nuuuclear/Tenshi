// Tiled extension script for tenshi's tilemaps

var tenshiMapFormat = {
    name: "Tenshi Tilemap",
    extension: "tmap",

    write: function(map, fileName) {
        if (map.infinite) {
            return "Tenshi Tilemap does not support infinite maps";
        }

        if (map.width <= 0 || map.height <= 0) {
            return "Invalid map dimensions";
        }

        if (map.width > 65535 || map.height > 65535) {
            return "Map dimensions cannot exceed 65535";
        }

        var width = map.width;
        var height = map.height;
        var cellCount = width * height;

        // Find the tileset asset name.
        var tilesetName = map.property("tenshiTileset");

        if (tilesetName === undefined 
        ||  tilesetName === null 
        ||  tilesetName === ""
        ) {
            if (map.tilesets.length === 1) {
                var automaticTileset = map.tilesets[0].tileset;

                if (automaticTileset) {
                    tilesetName = automaticTileset.fileName;
                } else {
                    tilesetName = map.tilesets[0].name;
                }
            } else {
                return "Property 'tenshiTileset' must be set";
            }
        }

        tilesetName = String(tilesetName);

        // map name
        var mapName = map.property("tenshiName");

        if (mapName === undefined ||
            mapName === null ||
            mapName === "") {

            mapName = map.name;
        }

        mapName = String(mapName);

        // layer
        var layers = [];

        for (var layerIndex = 0; layerIndex < map.layerCount; ++layerIndex) {

            var layer = map.layerAt(layerIndex);

            if (!layer.isTileLayer)
                continue;

            if (layer.name === "Collision")
                continue;

            if (layer.name.endsWith(" [Attributes]"))
                continue;

            layers.push(layer);
        }

        if (layers.length === 0) {
            return "At least one tile layer is needed for the map";
        }

        if (layers.length > 65535) { // why would anyone need this many!!!!
            return "Too many tile layers";
        }

        // collider layer
        var collisionLayer = null;

        for (var i = 0;
             i < map.layerCount;
             ++i) {

            var candidate = map.layerAt(i);

            if (candidate.isTileLayer &&
                candidate.name === "Collision") {

                collisionLayer = candidate;
                break;
            }
        }

        var collision = new Uint8Array(cellCount);

        if (collisionLayer) {
            for (var y = 0; y < height; ++y) {
                for (var x = 0; x < width; ++x) {

                    var cell = collisionLayer.cellAt(x, y);

                    if (!cell || cell.tileId < 0)
                        continue;

                    var collider = getTileProperty(
                        collisionLayer,
                        x,
                        y,
                        "tenshiCollider"
                    );

                    collision[y * width + x] 
                    = clampUInt8(
                        collider,
                        0
                    );
                }
            }
        }

        // build drawn map layers
        var outputLayers = [];

        for (var i = 0; i < layers.length; ++i) {
            var layer = layers[i];
            var tiles = new Uint16Array(cellCount);
            var attributes = new Uint8Array(cellCount);

            // 0xFFFF is air
            for (var n = 0; n < cellCount; ++n) {
                tiles[n] = 0xFFFF;
            }

            for (var y = 0; y < height; ++y) {
                for (var x = 0; x < width; ++x) {

                    var cell = layer.cellAt(x, y);
                    var index = y * width + x;

                    if (!cell || cell.tileId < 0) {
                        tiles[index] = 0xFFFF;
                        attributes[index] = 0;
                        continue;
                    }

                    var tileId = cell.tileId;

                    if (tileId > 0xFFFE) {
                        return "ID of a tile exceeds intager range in layer '" 
                            +   layer.name 
                            +   "'.";
                    }

                    tiles[index] = tileId;
                    var flags = getTileProperty(
                        layer,
                        x,
                        y,
                        "tenshiFlags"
                    );

                    attributes[index] = clampUInt8(
                        flags,
                        0
                    );
                }
            }

            outputLayers.push({
                depth: i,
                tiles: tiles,
                attributes: attributes
            });
        }

        // doors
        var entries = collectTransitions(
            map,
            "Entries",
            width,
            height
        );
        var exits = collectTransitions(
            map,
            "Exits",
            width,
            height
        );

        // build strings
        var stringData = [];

        var mapNameBytes = utf8Bytes(mapName);
        var tilesetNameBytes = utf8Bytes(tilesetName);
        var mapNameOffset = 0;

        for (var i = 0; i < mapNameBytes.length; ++i) {
            stringData.push(mapNameBytes[i]);
        }

        var tilesetNameOffset = stringData.length;

        for (var i = 0; i < tilesetNameBytes.length; ++i) {
            stringData.push(tilesetNameBytes[i]);
        }

        var HEADER_SIZE = 48;
        var TRANSITION_SIZE = 8;
        var LAYER_SIZE = 18;

        var entriesOffset 
        =   HEADER_SIZE;

        var exitsOffset 
        =   entriesOffset 
        +   entries.length 
        *   TRANSITION_SIZE;

        var layersOffset 
        =   exitsOffset 
        +   exits.length 
        *   TRANSITION_SIZE;

        var collisionOffset 
        =   layersOffset 
        +   outputLayers.length 
        *   LAYER_SIZE;

        var stringsOffset 
        =   collisionOffset 
        +   collision.byteLength;

        var dataOffset 
        =   stringsOffset 
        +   stringData.length;

        // data offsets for layer
        var layerHeaders = [];

        var currentDataOffset = dataOffset;

        for (var i = 0; i < outputLayers.length; ++i) {
            var layer = outputLayers[i];

            var tileDataOffset = currentDataOffset;
            var tileDataSize = layer.tiles.byteLength;

            currentDataOffset += tileDataSize;

            var attributeDataOffset = currentDataOffset;
            var attributeDataSize = layer.attributes.byteLength;

            currentDataOffset += attributeDataSize;

            layerHeaders.push({
                depth: layer.depth,
                tileDataOffset: tileDataOffset,
                attributeDataOffset: attributeDataOffset,
                tileDataSize: tileDataSize,
                attributeDataSize: attributeDataSize
            });
        }

        // make complete buffer output
        var totalSize = currentDataOffset;

        var output = new ArrayBuffer(totalSize);
        var view = new DataView(output);
        var bytes = new Uint8Array(output);

        // header
        writeMagic(
            bytes,
            0,
            "TMAP"
        );

        view.setUint16(
            4,
            1,
            true
        );

        view.setUint16(
            6,
            width,
            true
        );

        view.setUint16(
            8,
            height,
            true
        );

        view.setUint16(
            10,
            outputLayers.length,
            true
        );

        view.setUint16(
            12,
            entries.length,
            true
        );

        view.setUint16(
            14,
            exits.length,
            true
        );

        view.setUint32(
            16,
            stringsOffset +
            mapNameOffset,
            true
        );

        view.setUint32(
            20,
            mapNameBytes.length,
            true
        );

        view.setUint32(
            24,
            stringsOffset +
            tilesetNameOffset,
            true
        );

        view.setUint32(
            28,
            tilesetNameBytes.length,
            true
        );

        view.setUint32(
            32,
            entriesOffset,
            true
        );

        view.setUint32(
            36,
            exitsOffset,
            true
        );

        view.setUint32(
            40,
            layersOffset,
            true
        );

        view.setUint32(
            44,
            collisionOffset,
            true
        );

        // entries
        var offset = entriesOffset;

        for (var i = 0; i < entries.length; ++i) {
            view.setUint32(
                offset,
                entries[i].id,
                true
            );

            view.setUint32(
                offset + 4,
                entries[i].tileIndex,
                true
            );

            offset += TRANSITION_SIZE;
        }

        // exits
        offset = exitsOffset;

        for (var i = 0; i < exits.length; ++i) {
            view.setUint32(
                offset,
                exits[i].id,
                true
            );

            view.setUint32(
                offset + 4,
                exits[i].tileIndex,
                true
            );

            offset += TRANSITION_SIZE;
        }

        // layer table
        offset = layersOffset;

        for (var i = 0; i < layerHeaders.length; ++i) {
            var layer = layerHeaders[i];

            view.setInt16(
                offset,
                layer.depth,
                true
            );

            view.setUint32(
                offset + 2,
                layer.tileDataOffset,
                true
            );

            view.setUint32(
                offset + 6,
                layer.attributeDataOffset,
                true
            );

            view.setUint32(
                offset + 10,
                layer.tileDataSize,
                true
            );

            view.setUint32(
                offset + 14,
                layer.attributeDataSize,
                true
            );

            offset += LAYER_SIZE;
        }

        // collider
        bytes.set(
            collision,
            collisionOffset
        );

        // string
        bytes.set(
            new Uint8Array(mapNameBytes),
            stringsOffset +
            mapNameOffset
        );

        bytes.set(
            new Uint8Array(tilesetNameBytes),
            stringsOffset +
            tilesetNameOffset
        );

        // layer
        for (var i = 0; i < outputLayers.length; ++i) {
            var layer = outputLayers[i];
            var header = layerHeaders[i];

            bytes.set(
                new Uint8Array(layer.tiles.buffer),
                header.tileDataOffset
            );

            bytes.set(
                new Uint8Array(layer.attributes.buffer),
                header.attributeDataOffset
            );
        }

        // write file
        var file = new BinaryFile(
            fileName,
            BinaryFile.WriteOnly
        );

        file.write(output);
        file.commit();
    }
};

function writeMagic(bytes, offset, magic) {
    for (var i = 0; i < 4; ++i) {
        bytes[offset + i] = magic.charCodeAt(i);
    }
}

function utf8Bytes(str) {
    // return Array.from(new TextEncoder().encode(str));
    var bytes = [];

    for (var i = 0; i < str.length; ++i) {
        var c = str.charCodeAt(i);

        if (c < 0x80) {
            bytes.push(c);
        } else if (c < 0x800) {
            bytes.push(
                0xC0 | (c >> 6),
                0x80 | (c & 0x3F)
            );
        } else if (c >= 0xD800 && c <= 0xDBFF) {
            var high = c;
            var low = str.charCodeAt(++i);

            if (low >= 0xDC00 && low <= 0xDFFF) {
                var codePoint 
                =   0x10000
                +   ((high - 0xD800) << 10)
                +   (low - 0xDC00);

                bytes.push(
                    0xF0 | (codePoint >> 18),
                    0x80 | ((codePoint >> 12) & 0x3F),
                    0x80 | ((codePoint >> 6) & 0x3F),
                    0x80 | (codePoint & 0x3F)
                );
            } else {
                bytes.push(
                    0xEF, 0xBF, 0xBD
                );
                --i;
            }
        } else if (c >= 0xDC00 && c <= 0xDFFF) {
            bytes.push(
                0xEF, 0xBF, 0xBD
            );
        } else {
            bytes.push(
                0xE0 | (c >> 12),
                0x80 | ((c >> 6) & 0x3F),
                0x80 | (c & 0x3F)
            );
        }
    }

    return new Uint8Array(bytes);
}

function clampUInt8(value, fallback) {
    if (value === undefined 
    ||  value === null 
    ||  value === ""
    ) {
        return fallback;
    }

    var number = Number(value);

    if (!Number.isFinite(number))
        return fallback;

    number = Math.floor(number);

    if (number < 0)
        return 0;

    if (number > 255)
        return 255;

    return number;
}

function getTileProperty(layer, x, y, propertyName) {
    var cell = layer.cellAt(x, y);

    if (!cell || cell.tileId < 0) {
        return 0;
    }

    var tile = layer.tileAt(x, y);

    if (!tile)
        return 0;

    var value = tile.property(propertyName);
    if (value === undefined 
    ||  value === null
    ) {

        return 0;
    }

    return value;
}

function collectTransitions(map, layerName, width, height) {
    var result = [];
    for (var i = 0; i < map.layerCount; ++i) {
        var layer = map.layerAt(i);

        if (!layer.isObjectLayer)
            continue;

        if (layer.name !== layerName)
            continue;

        for (var j = 0; j < layer.objectCount; ++j) {
            var object = layer.objectAt(j);

            // objects are positioned in pixels
            var x = Math.floor(
                object.x 
                /   map.tileWidth
            );
            var y = Math.floor(
                object.y 
                /   map.tileHeight
            );

            // point outside the map is invalid
            if (x < 0 
            ||  y < 0 
            ||  x >= width 
            ||  y >= height
            ) {
                tiled.warn(
                    "Ignoring " 
                +   layerName 
                +   " object outside map: " 
                +   object.name
                );

                continue;
            }

            result.push({
                id: object.id >>> 0,
                tileIndex: (y * width + x) >>> 0
            });
        }
    }

    return result;
}

// register it with tiled
tiled.registerMapFormat(
    "tenshi-tmap",
    tenshiMapFormat
);