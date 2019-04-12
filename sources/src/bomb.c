/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 9 by ELFAIZMED
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb {
	int x, y;
	int range ;
    int ttl;
		
};
