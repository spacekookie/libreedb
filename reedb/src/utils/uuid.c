/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "../../include/reedb/utils/uuid.h"

/* 
 * forward definitions
 */
G_DEFINE_TYPE (uuid, uuid, G_TYPE_OBJECT);
/*
/* forward declarations of default virtual methods 
 */

static void
uuid_dispose (GObject *gobject)
{
  uuid *self = UUID (gobject);

  /* 
   * In dispose, you are supposed to free all types referenced from this
   * object which might themselves hold a reference to self. Generally,
   * the most simple solution is to unref all members on which you own a 
   * reference.
   */

  /* Chain up to the parent class */
  G_OBJECT_CLASS (uuid_parent_class)->dispose (gobject);
}

static void
uuid_finalize (GObject *gobject)
{
  uuid *self = UUID (gobject);

  /* Chain up to the parent class */
  G_OBJECT_CLASS (uuid_parent_class)->finalize (gobject);
}

static void
uuid_init (uuid *self)
{
  /* initialize all public and private members to reasonable default values. */

  /* Default implementations for virtual methods 
   * For pure-virtual functions, set these to NULL
   */
}
