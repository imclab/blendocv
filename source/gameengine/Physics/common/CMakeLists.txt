# $Id: CMakeLists.txt 37030 2011-05-31 01:15:44Z campbellbarton $
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): Jacques Beaurain.
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	.
	../Dummy
)

set(INC_SYS

)

set(SRC
	PHY_IMotionState.cpp
	PHY_IController.cpp
	PHY_IPhysicsController.cpp
	PHY_IGraphicController.cpp
	PHY_IPhysicsEnvironment.cpp
	PHY_IVehicle.cpp

	PHY_DynamicTypes.h
	PHY_IController.h
	PHY_IGraphicController.h
	PHY_IMotionState.h
	PHY_IPhysicsController.h
	PHY_IPhysicsEnvironment.h
	PHY_IVehicle.h
	PHY_Pro.h
)

blender_add_lib(ge_phys_common "${SRC}" "${INC}" "${INC_SYS}")
