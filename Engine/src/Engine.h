#pragma once

#include "core\Application.h"
#include "core\Window.h"

#include "core\camera\Camera.h"

#include "core\game_objects\GameObject.h"
#include "core\game_objects\cube_map\CubeMap.h"
#include "core\game_objects\lights\DirLight.h"
#include "core\game_objects\lights\Light.h"
#include "core\game_objects\lights\PointLight.h"
#include "core\game_objects\particle\Particle.h"
#include "core\game_objects\particle\ParticleMaster.h"
#include "core\game_objects\particle\ParticleRenderer.h"
#include "core\game_objects\particle\ParticleSystem.h"

#include "core\util\framebuffer\FrameBuffer.h"
#include "core\util\gui\GuiRenderer.h"
#include "core\util\gui\GuiTexture.h"
#include "core\util\gui\Quad.h"
#include "core\util\input\InputManager.h"
#include "core\util\mesh\Mesh.h"
#include "core\util\mesh\Model.h"
#include "core\util\mesh\CubeModel.h"
#include "core\util\shaders\ShaderProgram.h"
#include "core\util\shaders\basic\BasicShader.h"
#include "core\util\shaders\gui\GuiShader.h"
#include "core\util\shaders\lamp\LampShader.h"
#include "core\util\shaders\lighting\LightingShader.h"
#include "core\util\shaders\particle\ParticleShader.h"
#include "core\util\texture\Texture.h"
#include "core\util\util\Util.h"
#include "core\util\Logger.h"
//---------------------------------------------------------//