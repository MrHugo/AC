enum                            // block types, order matters!
{
    SOLID = 0,                  // entirely solid cube [only specifies wtex]
    CORNER,                     // half full corner of a wall
    FHF,                        // floor heightfield using neighbour vdelta values
    CHF,                        // idem ceiling
    SPACE,                      // entirely empty cube
    SEMISOLID,                  // generated by mipmapping
    MAXTYPE
};

struct sqr
{
    uchar type;                 // one of the above
    char floor, ceil;           // height, in cubes
    uchar wtex, ftex, ctex;     // wall/floor/ceil texture ids
    uchar r, g, b;              // light value at upper left vertex
    uchar vdelta;               // vertex delta, used for heightfield cubes
    char defer;                 // used in mipmapping, when true this cube is not a perfect mip
    char occluded;              // true when occluded
    uchar utex;                 // upper wall tex id
    uchar tag;                  // used by triggers
    uchar reserved[2];
};

enum                            // hardcoded texture numbers
{
    DEFAULT_SKY = 0,
    DEFAULT_LIQUID,
    DEFAULT_WALL,
    DEFAULT_FLOOR,
    DEFAULT_CEIL
};

enum                            // stuff encoded in sqr.tag
{
    TAGTRIGGERMASK = 0x3F,      // room for old fashioned cube 1 triggers
    TAGCLIP = 0x40,             // clips all objects
    TAGPLCLIP = 0x80            // clips only players
};
#define TAGANYCLIP (TAGCLIP|TAGPLCLIP)

#define MAXMAPVERSION 10        // defines the highest readable format
#define MAPVERSION 9            // default map format version to be written (bump if map format changes, see worldio.cpp)

struct header                   // map file format header
{
    char head[4];               // "CUBE"
    int version;                // any >8bit quantity is little endian
    int headersize;             // sizeof(header)
    int sfactor;                // in bits
    int numents;
    char maptitle[128];
    uchar texlists[3][256];
    int waterlevel;
    uchar watercolor[4];
    int maprevision;
    int ambient;
    int reserved[12];
    //char mediareq[128];         // version 7 and 8 only.
};

struct mapstats
{
    struct header hdr;
    int entcnt[MAXENTTYPES];
    int cgzsize;
    uchar *enttypes;
    short *entposs;
    int spawns[3];
    int flags[2];
    int flagents[2];
    bool hasffaspawns;
    bool hasteamspawns;
    bool hasflags;
};

struct mapdim
{   //   0   2   1   3     6         7
    int x1, x2, y1, y2, minfloor, maxceil;       // outer borders (points to last used cube)
    //    4      5
    int xspan, yspan;                            // size of area between x1|y1 and x2|y2
    float xm, ym;                                // middle of the map
};

#define SWS(w,x,y,s) (&(w)[((y)<<(s))+(x)])
#define SW(w,x,y) SWS(w,x,y,sfactor)
#define S(x,y) SW(world,x,y)            // convenient lookup of a lowest mip cube
#define SMALLEST_FACTOR 6               // determines number of mips there can be
#define DEFAULT_FACTOR 8
#define LARGEST_FACTOR 11               // 10 is already insane
#define MAXENTITIES 65535
#define MAXHEADEREXTRA (1<<20)
#define SOLID(x) ((x)->type==SOLID)
#define MINBORD 2                       // 2 cubes from the edge of the world are always solid
#define OUTBORD(x,y) ((x)<MINBORD || (y)<MINBORD || (x)>=ssize-MINBORD || (y)>=ssize-MINBORD)
#define OUTBORDRAD(x,y,rad) (int(x-rad)<MINBORD || int(y-rad)<MINBORD || int(x+rad)>=ssize-MINBORD || (y+rad)>=ssize-MINBORD)
#define MAXMHEIGHT 30
#define MAXMAREA 10000
#define MAXHHITS 50000                  // was 6000, which denied my fav. maps - jamz, 2012-06-12; 15000 denies sane map too - lucas
#define MINFF 2500

struct block { int x, y, xs, ys, h; short p[5]; };

// vertex array format

struct vertex { float u, v, x, y, z; uchar r, g, b, a; };

