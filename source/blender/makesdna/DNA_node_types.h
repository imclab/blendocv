/*
 * $Id: DNA_node_types.h 40295 2011-09-17 13:03:42Z blendix $ 
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Bob Holcomb, Xavier Thomas
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifndef DNA_NODE_TYPES_H
#define DNA_NODE_TYPES_H

/** \file DNA_node_types.h
 *  \ingroup DNA
 */

#include "DNA_ID.h"
#include "DNA_vec_types.h"
#include "DNA_listBase.h"
//#include <cv.h>

struct ID;
struct ListBase;
struct SpaceNode;
struct bNodeLink;
struct bNodeType;
struct bNodeTreeExec;
struct AnimData;
struct bGPdata;
struct uiBlock;

#define NODE_MAXSTR 32

typedef struct bNodeStack {
	float vec[4];
	float min, max;
	void *data;
	short hasinput;			/* when input has link, tagged before executing */
	short hasoutput;		/* when output is linked, tagged before executing */
	short datatype;			/* type of data pointer */
	short sockettype;		/* type of socket stack comes from, to remap linking different sockets */
	short is_copy;			/* data is a copy of external data (no freeing) */
	short external;			/* data is used by external nodes (no freeing) */
	short pad[2];
} bNodeStack;

/* ns->datatype, shadetree only */
#define NS_OSA_VECTORS		1
#define NS_OSA_VALUES		2

typedef struct bNodeSocket {
	struct bNodeSocket *next, *prev, *new_sock;
	
	char name[32];
	
	void *storage;				/* custom storage */
	
	short type, flag;
	short limit;				/* max. number of links */
	short pad1;
	
	float locx, locy;
	
	void *default_value;		/* default input value used for unlinked sockets */
	
	/* execution data */
	short stack_index;			/* local stack index */
	short stack_type;			/* deprecated, kept for forward compatibility */
	int pad3;
	void *cache;				/* cached data from execution */
	
	/* internal data to retrieve relations and groups */
	int own_index;				/* group socket identifiers, to find matching pairs after reading files */
	int to_index;				/* XXX deprecated, only used for restoring old group node links */
	struct bNodeSocket *groupsock;
	
	struct bNodeLink *link;		/* a link pointer, set in ntreeUpdateTree */

	/* DEPRECATED only needed for do_versions */
	bNodeStack ns;				/* custom data for inputs, only UI writes in this */
} bNodeSocket;

/* sock->type */
#define SOCK_FLOAT			0
#define SOCK_VECTOR			1
#define SOCK_RGBA			2
#define SOCK_SHADER			3
#define SOCK_BOOLEAN		4
#define SOCK_MESH			5
#define SOCK_INT			6
/*OpenCV sock Types May be don't used*/
#define SOCK_OCV_IMAGE                  7
#define SOCK_OCV_POINT                  8
#define SOCK_OCV_POINT2D32f             9
#define SOCK_OCV_POINT3D32f             10
#define SOCK_OCV_POINT2D64f             11
#define SOCK_OCV_POINT3D64f             12
#define SOCK_OCV_SIZE                   13
#define SOCK_OCV_SIZE2D32f              14
#define SOCK_OCV_RECT                   15
#define SOCK_OCV_SCALAR                 16
#define SOCK_OCV_TERMCRITERIA   		17
#define SOCK_OCV_MAT                    18
#define SOCK_OCV_MATND                  19
#define SOCK_OCV_SPARSEMAT              20
#define SOCK_OCV_ARR                    21
#define SOCK_OCV_SEQ                    22
#define SOCK_OCV_BOX                    23
#define SOCK_OCV_VIDEO                  24
#define NUM_SOCKET_TYPES	25	/* must be last! */



/* socket side (input/output) */
#define SOCK_IN		1
#define SOCK_OUT	2

/* sock->flag, first bit is select */
	/* hidden is user defined, to hide unused */
#define SOCK_HIDDEN				2
	/* only used now for groups... */
#define SOCK_IN_USE				4	/* XXX deprecated */
	/* unavailable is for dynamic sockets */
#define SOCK_UNAVAIL			8
	/* dynamic socket (can be modified by user) */
#define SOCK_DYNAMIC			16
	/* group socket should not be exposed */
#define SOCK_INTERNAL			32
	/* socket collapsed in UI */
#define SOCK_COLLAPSED			64
	/* hide socket value, if it gets auto default */
#define SOCK_HIDE_VALUE			128

typedef struct bNodePreview {
	unsigned char *rect;
	short xsize, ysize;
	int pad;
} bNodePreview;

/* limit data in bNode to what we want to see saved? */
typedef struct bNode {
	struct bNode *next, *prev, *new_node;
	
	char name[32];
	short type, flag;
	short done, level;		/* both for dependency and sorting */
	short lasty, menunr;	/* lasty: check preview render status, menunr: browse ID blocks */
	short stack_index;		/* for groupnode, offset in global caller stack */
	short nr;				/* number of this node in list, used for UI exec events */
	int error;
        int dummy; //only for get complete 64 bit processor optimized
        
	ListBase inputs, outputs;
	struct bNode *parent;	/* parent node */
	struct ID *id;			/* optional link to libdata */
	void *storage;			/* custom data, must be struct, for storage in file */
	
	float locx, locy;		/* root offset for drawing */
	float width, height;	/* node custom width and height */
	float miniwidth;		/* node width if hidden */
	int pad;
	char label[32];			/* custom user-defined label */
	short custom1, custom2;	/* to be abused for buttons */
	float custom3, custom4;
	
	short need_exec, exec;	/* need_exec is set as UI execution event, exec is flag during exec */
	void *threaddata;		/* optional extra storage for use in thread (read only then!) */
	
	rctf totr;				/* entire boundbox */
	rctf butr;				/* optional buttons area */
	rctf prvr;				/* optional preview area */
	bNodePreview *preview;	/* optional preview image */
	struct uiBlock *block;	/* runtime during drawing */
	
	struct bNodeType *typeinfo;	/* lookup of callbacks and defaults */
} bNode;

/* node->flag */
#define NODE_SELECT			1
#define NODE_OPTIONS		2
#define NODE_PREVIEW		4
#define NODE_HIDDEN			8
#define NODE_ACTIVE			16
#define NODE_ACTIVE_ID		32
#define NODE_DO_OUTPUT		64
#define NODE_GROUP_EDIT		128
	/* free test flag, undefined */
#define NODE_TEST			256
	/* composite: don't do node but pass on buffer(s) */
#define NODE_MUTED			512
#define NODE_CUSTOM_NAME	1024	/* deprecated! */
	/* group node types: use const outputs by default */
#define NODE_CONST_OUTPUT	(1<<11)
	/* node is always behind others */
#define NODE_BACKGROUND		(1<<12)
	/* automatic flag for nodes included in transforms */
#define NODE_TRANSFORM		(1<<13)

typedef struct bNodeLink {
	struct bNodeLink *next, *prev;
	
	bNode *fromnode, *tonode;
	bNodeSocket *fromsock, *tosock;
	
	int flag;
	int pad;
} bNodeLink;

/* link->flag */
#define NODE_LINKFLAG_HILITE	1		/* link has been successfully validated */
#define NODE_LINK_VALID			2

/* the basis for a Node tree, all links and nodes reside internal here */
/* only re-usable node trees are in the library though, materials and textures allocate own tree struct */
typedef struct bNodeTree {
	ID id;
	struct AnimData *adt;		/* animation data (must be immediately after id for utilities to use it) */ 
	
	struct bGPdata *gpd;		/* grease pencil data */
	
	ListBase nodes, links;
	
	int type, init;					/* set init on fileread */
	int cur_index;					/* sockets in groups have unique identifiers, adding new sockets always 
									   will increase this counter */
	int flag;
	int update;						/* update flags */
	
	int nodetype;					/* specific node type this tree is used for */
	
	ListBase inputs, outputs;		/* external sockets for group nodes */
	
	/* execution data */
	/* XXX It would be preferable to completely move this data out of the underlying node tree,
	 * so node tree execution could finally run independent of the tree itself. This would allow node trees
	 * to be merely linked by other data (materials, textures, etc.), as ID data is supposed to.
	 * Execution data is generated from the tree once at execution start and can then be used
	 * as long as necessary, even while the tree is being modified.
	 */
	struct bNodeTreeExec *execdata;
	
	/* callbacks */
	void (*progress)(void *, float progress);
	void (*stats_draw)(void *, char *str);
	int (*test_break)(void *);
	void *tbh, *prh, *sdh;
	
} bNodeTree;

/* ntree->type, index */
#define NTREE_SHADER		0
#define NTREE_COMPOSIT		1
#define NTREE_TEXTURE		2
#define NUM_NTREE_TYPES		3

/* ntree->init, flag */
#define NTREE_TYPE_INIT		1

/* ntree->flag */
#define NTREE_DS_EXPAND		1	/* for animation editors */
/* XXX not nice, but needed as a temporary flags
 * for group updates after library linking.
 */
#define NTREE_DO_VERSIONS_GROUP_EXPOSE	1024

/* ntree->update */
#define NTREE_UPDATE			0xFFFF	/* generic update flag (includes all others) */
#define NTREE_UPDATE_LINKS		1		/* links have been added or removed */
#define NTREE_UPDATE_NODES		2		/* nodes or sockets have been added or removed */
#define NTREE_UPDATE_GROUP_IN	16		/* group inputs have changed */
#define NTREE_UPDATE_GROUP_OUT	32		/* group outputs have changed */
#define NTREE_UPDATE_GROUP		48		/* group has changed (generic flag including all other group flags) */


/* socket value structs for input buttons */

typedef struct bNodeSocketValueInt {
	int subtype;				/* RNA subtype */
	int value;
	int min, max;
} bNodeSocketValueInt;

typedef struct bNodeSocketValueFloat {
	int subtype;				/* RNA subtype */
	float value;
	float min, max;
} bNodeSocketValueFloat;

typedef struct bNodeSocketValueBoolean {
	char value;
	char pad[3];
} bNodeSocketValueBoolean;

typedef struct bNodeSocketValueVector {
	int subtype;				/* RNA subtype */
	float value[3];
	float min, max;
} bNodeSocketValueVector;

typedef struct bNodeSocketValueRGBA {
	float value[4];
} bNodeSocketValueRGBA;
/*
typedef struct bNodeSocketValueCvPoint {
    CvPoint value;
} bNodeSocketValueCvPoint;

typedef struct bNodeSocketValueCvImage {
    IplImage value;
} bNodeSocketValueCvImage;
*/

/* data structs, for node->storage */

/* this one has been replaced with ImageUser, keep it for do_versions() */
typedef struct NodeImageAnim {
	int frames, sfra, nr;
	char cyclic, movie;
	short pad;
} NodeImageAnim;

typedef struct NodeBlurData {
	short sizex, sizey;
	short samples, maxspeed, minspeed, relative, aspect;
	short curved;
	float fac, percentx, percenty;
	short filtertype;
	char bokeh, gamma;
	int image_in_width, image_in_height; /* needed for absolute/relative conversions */
} NodeBlurData;

typedef struct NodeDBlurData {
	float center_x, center_y, distance, angle, spin, zoom;
	short iter;
	char wrap, pad;
} NodeDBlurData;

typedef struct NodeBilateralBlurData {
	float sigma_color, sigma_space;
	short iter, pad;
} NodeBilateralBlurData;

typedef struct NodeHueSat {
	float hue, sat, val;
} NodeHueSat;

typedef struct NodeImageFile {
	char name[256];
	short imtype, subimtype, quality, codec;
	int sfra, efra;
} NodeImageFile;

typedef struct NodeChroma {
	float t1,t2,t3;
	float fsize,fstrength,falpha;
	float key[4];
   short algorithm, channel;
} NodeChroma;

typedef struct NodeTwoXYs {
	short x1, x2, y1, y2;
   float fac_x1, fac_x2, fac_y1, fac_y2;
} NodeTwoXYs;

typedef struct NodeTwoFloats {
	float x, y;
} NodeTwoFloats;

typedef struct NodeGeometry {
	char uvname[32];
	char colname[32];
} NodeGeometry;

typedef struct NodeVertexCol {
	char name[32];
} NodeVertexCol;

/* qdn: Defocus blur node */
typedef struct NodeDefocus {
	char bktype, rotation, preview, gamco;
	short samples, no_zbuf;
	float fstop, maxblur, bthresh, scale;
} NodeDefocus;

typedef struct NodeScriptDict {
	void *dict; /* for PyObject *dict */
	void *node; /* for BPy_Node *node */
} NodeScriptDict;

/* qdn: glare node */
typedef struct NodeGlare {
	char quality, type, iter;
	char angle, angle_ofs, size, pad[2];
	float colmod, mix, threshold, fade;
} NodeGlare;

/* qdn: tonemap node */
typedef struct NodeTonemap {
	float key, offset, gamma;
	float f, m, a, c;
	int type;
} NodeTonemap;

/* qdn: lens distortion node */
typedef struct NodeLensDist {
	short jit, proj, fit, pad;
} NodeLensDist;

typedef struct NodeColorBalance {
	/* for processing */
	float slope[3];
	float offset[3];
	float power[3];
	
	/* for ui representation */
	float lift[3];
	float gamma[3];
	float gain[3];

	/* temp storage for inverted lift */
	float lift_lgg[3];
	float gamma_inv[3];
} NodeColorBalance;

typedef struct NodeColorspill {
	short limchan, unspill;
	float limscale;
	float uspillr, uspillg, uspillb;
}NodeColorspill;

/* TEX_output */
typedef struct TexNodeOutput {
	char name[32];
} TexNodeOutput;

/* comp channel matte */
#define CMP_NODE_CHANNEL_MATTE_CS_RGB	1
#define CMP_NODE_CHANNEL_MATTE_CS_HSV	2
#define CMP_NODE_CHANNEL_MATTE_CS_YUV	3
#define CMP_NODE_CHANNEL_MATTE_CS_YCC	4

#define CMP_NODE_BLUR_ASPECT_NONE		0
#define CMP_NODE_BLUR_ASPECT_Y			1
#define CMP_NODE_BLUR_ASPECT_X			2

#endif
