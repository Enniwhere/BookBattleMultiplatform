/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCTMX_LAYER_H__
#define __CCTMX_LAYER_H__

#include "CCTMXObjectGroup.h"
#include "base_nodes/CCAtlasNode.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "CCTMXXMLParser.h"
NS_CC_BEGIN

class TMXMapInfo;
class TMXLayerInfo;
class TMXTilesetInfo;
struct _ccCArray;

/**
 * @addtogroup tilemap_parallax_nodes
 * @{
 */

/** @brief TMXLayer represents the TMX layer.

It is a subclass of SpriteBatchNode. By default the tiles are rendered using a TextureAtlas.
If you modify a tile on runtime, then, that tile will become a Sprite, otherwise no Sprite objects are created.
The benefits of using Sprite objects as tiles are:
- tiles (Sprite) can be rotated/scaled/moved with a nice API

If the layer contains a property named "cc_vertexz" with an integer (in can be positive or negative),
then all the tiles belonging to the layer will use that value as their OpenGL vertex Z for depth.

On the other hand, if the "cc_vertexz" property has the "automatic" value, then the tiles will use an automatic vertex Z value.
Also before drawing the tiles, GL_ALPHA_TEST will be enabled, and disabled after drawing them. The used alpha func will be:

glAlphaFunc( GL_GREATER, value )

"value" by default is 0, but you can change it from Tiled by adding the "cc_alpha_func" property to the layer.
The value 0 should work for most cases, but if you have tiles that are semi-transparent, then you might want to use a different
value, like 0.5.

For further information, please see the programming guide:

http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps

@since v0.8.1
Tiles can have tile flags for additional properties. At the moment only flip horizontal and flip vertical are used. These bit flags are defined in TMXXMLParser.h.

@since 1.1
*/

class CC_DLL TMXLayer : public SpriteBatchNode
{
public:
    /** creates a TMXLayer with an tileset info, a layer info and a map info */
    static TMXLayer * create(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);
    
    TMXLayer();
    virtual ~TMXLayer();

    /** initializes a TMXLayer with a tileset info, a layer info and a map info */
    bool initWithTilesetInfo(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo);

    /** dealloc the map that contains the tile position from memory.
    Unless you want to know at runtime the tiles positions, you can safely call this method.
    If you are going to call layer->tileGIDAt() then, don't release the map
    */
    void releaseMap();

    /** returns the tile (Sprite) at a given a tile coordinate.
    The returned Sprite will be already added to the TMXLayer. Don't add it again.
    The Sprite can be treated like any other Sprite: rotated, scaled, translated, opacity, color, etc.
    You can remove either by calling:
    - layer->removeChild(sprite, cleanup);
    - or layer->removeTileAt(Point(x,y));
    */
    Sprite* getTileAt(const Point& tileCoordinate);
    CC_DEPRECATED_ATTRIBUTE Sprite* tileAt(const Point& tileCoordinate) { return getTileAt(tileCoordinate); };
    
    /** returns the tile gid at a given tile coordinate. It also returns the tile flags.
     This method requires the the tile map has not been previously released (eg. don't call [layer releaseMap])
     */
    unsigned int getTileGIDAt(const Point& tileCoordinate, ccTMXTileFlags* flags = nullptr);
    CC_DEPRECATED_ATTRIBUTE unsigned int tileGIDAt(const Point& tileCoordinate, ccTMXTileFlags* flags = nullptr){
        return getTileGIDAt(tileCoordinate, flags);
    };

    /** sets the tile gid (gid = tile global id) at a given tile coordinate.
    The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
    If a tile is already placed at that position, then it will be removed.
    */
    void setTileGID(unsigned int gid, const Point& tileCoordinate);

    /** sets the tile gid (gid = tile global id) at a given tile coordinate.
     The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
     If a tile is already placed at that position, then it will be removed.
     
     Use withFlags if the tile flags need to be changed as well
     */

    void setTileGID(unsigned int gid, const Point& tileCoordinate, ccTMXTileFlags flags);

    /** removes a tile at given tile coordinate */
    void removeTileAt(const Point& tileCoordinate);

    /** returns the position in points of a given tile coordinate */
    Point getPositionAt(const Point& tileCoordinate);
    CC_DEPRECATED_ATTRIBUTE Point positionAt(const Point& tileCoordinate) { return getPositionAt(tileCoordinate); };

    /** return the value for the specific property name */
    String* getProperty(const char *propertyName) const;
    CC_DEPRECATED_ATTRIBUTE String* propertyNamed(const char *propertyName) const { return getProperty(propertyName); };

    /** Creates the tiles */
    void setupTiles();

    inline const char* getLayerName(){ return _layerName.c_str(); }
    inline void setLayerName(const char *layerName){ _layerName = layerName; }

    /** size of the layer in tiles */
    inline const Size& getLayerSize() const { return _layerSize; };
    inline void setLayerSize(const Size& size) { _layerSize = size; };
    
    /** size of the map's tile (could be different from the tile's size) */
    inline const Size& getMapTileSize() const { return _mapTileSize; };
    inline void setMapTileSize(const Size& size) { _mapTileSize = size; };
    
    /** pointer to the map of tiles */
    inline unsigned int* getTiles() const { return _tiles; };
    inline void setTiles(unsigned int* tiles) { _tiles = tiles; };
    
    /** Tileset information for the layer */
    inline TMXTilesetInfo* getTileSet() const { return _tileSet; };
    inline void setTileSet(TMXTilesetInfo* info) {
        CC_SAFE_RETAIN(info);
        CC_SAFE_RELEASE(_tileSet);
        _tileSet = info;
    };
    
    /** Layer orientation, which is the same as the map orientation */
    inline unsigned int getLayerOrientation() const { return _layerOrientation; };
    inline void setLayerOrientation(unsigned int orientation) { _layerOrientation = orientation; };
    
    /** properties from the layer. They can be added using Tiled */
    inline Dictionary* getProperties() const { return _properties; };
    inline void setProperties(Dictionary* properties) {
        CC_SAFE_RETAIN(properties);
        CC_SAFE_RELEASE(_properties);
        _properties = properties;
    };
    //
    // Override
    //
    /** TMXLayer doesn't support adding a Sprite manually.
     @warning addchild(z, tag); is not supported on TMXLayer. Instead of setTileGID.
     */
    virtual void addChild(Node * child, int zOrder, int tag) override;
    // super method
    void removeChild(Node* child, bool cleanup) override;


private:
    Point getPositionForIsoAt(const Point& pos);
    Point getPositionForOrthoAt(const Point& pos);
    Point getPositionForHexAt(const Point& pos);

    Point calculateLayerOffset(const Point& offset);

    /* optimization methods */
    Sprite* appendTileForGID(unsigned int gid, const Point& pos);
    Sprite* insertTileForGID(unsigned int gid, const Point& pos);
    Sprite* updateTileForGID(unsigned int gid, const Point& pos);

    /* The layer recognizes some special properties, like cc_vertez */
    void parseInternalProperties();
    void setupTileSprite(Sprite* sprite, Point pos, unsigned int gid);
    Sprite* reusedTileWithRect(Rect rect);
    int getVertexZForPos(const Point& pos);

    // index
    unsigned int atlasIndexForExistantZ(unsigned int z);
    unsigned int atlasIndexForNewZ(int z);
    
protected:
    //! name of the layer
    std::string _layerName;
    //! TMX Layer supports opacity
    unsigned char        _opacity;

    unsigned int        _minGID;
    unsigned int        _maxGID;

    //! Only used when vertexZ is used
    int                    _vertexZvalue;
    bool                _useAutomaticVertexZ;

    //! used for optimization
    Sprite            *_reusedTile;
    ccCArray            *_atlasIndexArray;
    
    // used for retina display
    float               _contentScaleFactor;
    
    /** size of the layer in tiles */
    Size _layerSize;
    /** size of the map's tile (could be different from the tile's size) */
    Size _mapTileSize;
    /** pointer to the map of tiles */
    unsigned int* _tiles;
    /** Tileset information for the layer */
    TMXTilesetInfo* _tileSet;
    /** Layer orientation, which is the same as the map orientation */
    unsigned int _layerOrientation;
    /** properties from the layer. They can be added using Tiled */
    Dictionary* _properties;
};

// end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif //__CCTMX_LAYER_H__

