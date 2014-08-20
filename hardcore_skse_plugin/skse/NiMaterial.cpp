#include "NiMaterial.h"
#include "GameAPI.h"
#include "GameObjects.h"

const _CreateShaderMaterial CreateShaderMaterial = (_CreateShaderMaterial)0x00C9A3E0;

/*
BSMaskedShaderMaterial * BSMaskedShaderMaterial::CreateFromLightingShader(BSLightingShaderMaterial * source)
{
	BSMaskedShaderMaterial * material = (BSMaskedShaderMaterial*)CreateShaderMaterial(BSMaskedShaderMaterial::kShaderType_FaceTint);
	if(material) {
		material->unk3C = source->unk3C;
		material->unk40 = source->unk40;
		material->unk44 = source->unk44;
		material->unk48 = source->unk48;
	}
	return material;
}
*/

void BSLightingShaderMaterial::SetTextureSet(BSTextureSet * newSet)
{
	textureSet = newSet;
}