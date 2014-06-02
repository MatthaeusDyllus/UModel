#ifndef __UNMATERIAL3_H__
#define __UNMATERIAL3_H__


/*
UE3 MATERIALS TREE:
~~~~~~~~~~~~~~~~~~~
	Surface (empty)
		Texture
			Texture2D
				ShadowMapTexture2D
				TerrainWeightMapTexture
				TextureFlipBook
			Texture2DComposite
			Texture3D
			TextureCube
			TextureMovie
			TextureRenderTarget
				TextureRenderTarget2D
				TextureRenderTargetCube
		MaterialInterface (empty)
			Material
				MaterialExpression
					MaterialExpressionTextureSample
					...
			MaterialInstance
				MaterialInstanceConstant
				MaterialInstanceTimeVarying
*/


#if UNREAL3

/*-----------------------------------------------------------------------------
	Unreal Engine 3 materials
-----------------------------------------------------------------------------*/

class UTexture3 : public UUnrealMaterial	// in UE3 it is derived from USurface->UObject; real name is UTexture
{
	DECLARE_CLASS(UTexture3, UUnrealMaterial)
public:
	float			UnpackMin[4];
	float			UnpackMax[4];
	FByteBulkData	SourceArt;

	UTexture3()
	{
		UnpackMax[0] = UnpackMax[1] = UnpackMax[2] = UnpackMax[3] = 1.0f;
	}

	BEGIN_PROP_TABLE
		PROP_FLOAT(UnpackMin)
		PROP_FLOAT(UnpackMax)
		// no properties required (all are for importing and cooking)
		PROP_DROP(SRGB)
		PROP_DROP(RGBE)
		PROP_DROP(CompressionNoAlpha)
		PROP_DROP(CompressionNone)
		PROP_DROP(CompressionNoMipmaps)
		PROP_DROP(CompressionFullDynamicRange)
		PROP_DROP(DeferCompression)
		PROP_DROP(NeverStream)
		PROP_DROP(bDitherMipMapAlpha)
		PROP_DROP(bPreserveBorderR)
		PROP_DROP(bPreserveBorderG)
		PROP_DROP(bPreserveBorderB)
		PROP_DROP(bPreserveBorderA)
		PROP_DROP(CompressionSettings)
		PROP_DROP(Filter)
		PROP_DROP(LODGroup)
		PROP_DROP(LODBias)
		PROP_DROP(SourceFilePath)
		PROP_DROP(SourceFileTimestamp)
		PROP_DROP(LightingGuid)
		PROP_DROP(AdjustRGBCurve)			// UDK
		PROP_DROP(AdjustSaturation)			// UDK
		PROP_DROP(AdjustBrightnessCurve)	// UDK
		PROP_DROP(MipGenSettings)
#if HUXLEY
		PROP_DROP(SourceArtWidth)
		PROP_DROP(SourceArtHeight)
#endif // HUXLEY
#if A51
		PROP_DROP(LODBiasWindows)
#endif // A51
#if BATMAN
		PROP_DROP(ForceOldCompression)
#endif // BATMAN
	END_PROP_TABLE

	virtual void Serialize(FArchive &Ar)
	{
		guard(UTexture3::Serialize);
		Super::Serialize(Ar);
#if TRANSFORMERS
		// Transformers: SourceArt is moved to separate class; but The Bourne Conspiracy has it (real ArLicenseeVer is unknown)
		if (Ar.Game == GAME_Transformers && Ar.ArLicenseeVer >= 100) return;
#endif
#if BORDERLANDS
		if (Ar.Game == GAME_Borderlands && Ar.ArVer >= 832) return;	// Borderlands 2; version unknown
#endif
#if APB
		if (Ar.Game == GAME_APB)
		{
			// APB has source art stored in separate bulk
			// here are 2 headers: 1 for SourceArt and 1 for TIndirectArray<BulkData> MipSourceArt
			// we can skip these blocks by skipping headers
			// each header is a magic 0x5D0E7707 + position in package
			Ar.Seek(Ar.Tell() + 8 * 2);
			return;
		}
#endif // APB
#if MASSEFF
		if (Ar.Game == GAME_MassEffect3) return;
#endif
#if BIOSHOCK3
		if (Ar.Game == GAME_Bioshock3) return;
#endif
		SourceArt.Serialize(Ar);
#if GUNLEGEND
		if (Ar.Game == GAME_GunLegend && Ar.ArVer >= 811)
		{
			// TIndirectArray<FByteBulkData> NonTopSourceArt
			int Count;
			Ar << Count;
			for (int i = 0; i < Count; i++)
			{
				FByteBulkData tmpBulk;
				tmpBulk.Skip(Ar);
			}
		}
#endif // GUNLEGEND
		unguard;
	}

#if RENDERING
	virtual bool IsTexture() const
	{
		return true;
	}
#endif // RENDERING
};

enum EPixelFormat
{
	PF_Unknown,
	PF_A32B32G32R32F,
	PF_A8R8G8B8,
	PF_G8,
	PF_G16,
	PF_DXT1,
	PF_DXT3,
	PF_DXT5,
	PF_UYVY,
	PF_FloatRGB,			// A RGB FP format with platform-specific implementation, for use with render targets
	PF_FloatRGBA,			// A RGBA FP format with platform-specific implementation, for use with render targets
	PF_DepthStencil,		// A depth+stencil format with platform-specific implementation, for use with render targets
	PF_ShadowDepth,			// A depth format with platform-specific implementation, for use with render targets
	PF_FilteredShadowDepth, // A depth format with platform-specific implementation, that can be filtered by hardware
	PF_R32F,
	PF_G16R16,
	PF_G16R16F,
	PF_G16R16F_FILTER,
	PF_G32R32F,
	PF_A2B10G10R10,
	PF_A16B16G16R16,
	PF_D24,
	PF_R16F,
	PF_R16F_FILTER,
	PF_BC5,
	PF_V8U8,
	PF_A1,
#if MASSEFF
	PF_NormalMap_LQ,
	PF_NormalMap_HQ,
#endif
#if THIEF4
	PF_BC7,
#endif
};

_ENUM(EPixelFormat)
{
	_E(PF_Unknown),
	_E(PF_A32B32G32R32F),
	_E(PF_A8R8G8B8),
	_E(PF_G8),
	_E(PF_G16),
	_E(PF_DXT1),
	_E(PF_DXT3),
	_E(PF_DXT5),
	_E(PF_UYVY),
	_E(PF_FloatRGB),
	_E(PF_FloatRGBA),
	_E(PF_DepthStencil),
	_E(PF_ShadowDepth),
	_E(PF_FilteredShadowDepth),
	_E(PF_R32F),
	_E(PF_G16R16),
	_E(PF_G16R16F),
	_E(PF_G16R16F_FILTER),
	_E(PF_G32R32F),
	_E(PF_A2B10G10R10),
	_E(PF_A16B16G16R16),
	_E(PF_D24),
	_E(PF_R16F),
	_E(PF_R16F_FILTER),
	_E(PF_BC5),
	_E(PF_V8U8),
	_E(PF_A1),
#if MASSEFF
	_E(PF_NormalMap_LQ),
	_E(PF_NormalMap_HQ),
#endif
#if THIEF4
	_E(PF_BC7),
#endif
};

enum ETextureFilter
{
	TF_Nearest,
	TF_Linear
};

enum ETextureAddress
{
	TA_Wrap,
	TA_Clamp,
	TA_Mirror
};

_ENUM(ETextureAddress)
{
	_E(TA_Wrap),
	_E(TA_Clamp),
	_E(TA_Mirror)
};

struct FTexture2DMipMap
{
	FByteBulkData	Data;	// FTextureMipBulkData
	int				SizeX;
	int				SizeY;

	friend FArchive& operator<<(FArchive &Ar, FTexture2DMipMap &Mip)
	{
		Mip.Data.Serialize(Ar);
#if DARKVOID
		if (Ar.Game == GAME_DarkVoid)
		{
			FByteBulkData DataX360Gamma;
			DataX360Gamma.Serialize(Ar);
		}
#endif // DARKVOID
		return Ar << Mip.SizeX << Mip.SizeY;
	}
};

class UTexture2D : public UTexture3
{
	DECLARE_CLASS(UTexture2D, UTexture3)
public:
	TArray<FTexture2DMipMap> Mips;
	TArray<FTexture2DMipMap> CachedETCMips;
	int				SizeX;
	int				SizeY;
	EPixelFormat	Format;
	ETextureAddress	AddressX;
	ETextureAddress	AddressY;
	FName			TextureFileCacheName;
	FGuid			TextureFileCacheGuid;
	int				MipTailBaseIdx;
	bool			bForcePVRTC4;		// iPhone

#if RENDERING
	// rendering implementation fields
	GLuint			TexNum;
#endif

#if RENDERING
	UTexture2D()
	:	TexNum(0)
	,	Format(PF_Unknown)
	,	AddressX(TA_Wrap)
	,	AddressY(TA_Wrap)
	,	bForcePVRTC4(false)
	{}
#endif

	BEGIN_PROP_TABLE
		PROP_INT(SizeX)
		PROP_INT(SizeY)
		PROP_ENUM2(Format, EPixelFormat)
		PROP_ENUM2(AddressX, ETextureAddress)
		PROP_ENUM2(AddressY, ETextureAddress)
		PROP_NAME(TextureFileCacheName)
		PROP_INT(MipTailBaseIdx)
		// drop unneeded props
		PROP_DROP(bGlobalForceMipLevelsToBeResident)
		PROP_DROP(OriginalSizeX)
		PROP_DROP(OriginalSizeY)
		PROP_BOOL(bForcePVRTC4)
		PROP_DROP(FirstResourceMemMip)
		PROP_DROP(SystemMemoryData)
#if FRONTLINES
		PROP_DROP(NumMips)
		PROP_DROP(SourceDataSizeX)
		PROP_DROP(SourceDataSizeY)
#endif // FRONTLINES
#if MASSEFF
		PROP_DROP(TFCFileGuid)
#endif
#if BATMAN
		// Batman 3
		PROP_DROP(MipTailInfo)
#endif
	END_PROP_TABLE

	virtual void Serialize(FArchive &Ar)
	{
		guard(UTexture2D::Serialize);
		Super::Serialize(Ar);
#if TERA
		if (Ar.Game == GAME_Tera && Ar.ArLicenseeVer >= 3)
		{
			FString SourceFilePath; // field from UTexture
			Ar << SourceFilePath;
		}
#endif // TERA
		if (Ar.ArVer < 297)
		{
			int Format2;
			Ar << SizeX << SizeY << Format2;
			Format = (EPixelFormat)Format2;		// int -> byte (enum)
		}
#if BORDERLANDS
		if (Ar.Game == GAME_Borderlands && Ar.ArLicenseeVer >= 46) Ar.Seek(Ar.Tell() + 16);	// Borderlands 1,2; some hash; version unknown!!
#endif
#if MKVSDC
		if (Ar.Game == GAME_MK && Ar.ArLicenseeVer >= 31)	//?? may be MidwayVer ?
		{
			FByteBulkData CustomMipSourceArt;
			CustomMipSourceArt.Skip(Ar);
			if (Ar.ArVer >= 573)		// Injustice, version unknown
			{
				int unk1, unk2;
				TArray<int> unk3;
				Ar << unk1 << unk2 << unk3;
			}
			Ar << Mips;
			goto skip_rest_quiet;		// Injustice has some extra mipmap arrays
		}
#endif // MKVSDC
		Ar << Mips;
#if BORDERLANDS
		if (Ar.Game == GAME_Borderlands && Ar.ArLicenseeVer >= 46) Ar.Seek(Ar.Tell() + 16);	// Borderlands 1,2; some hash; version unknown!!
#endif
#if MASSEFF
		if (Ar.Game >= GAME_MassEffect && Ar.Game <= GAME_MassEffect3)
		{
			int unkFC;
			if (Ar.ArLicenseeVer >= 65) Ar << unkFC;
			if (Ar.ArLicenseeVer >= 99) goto tfc_guid;
		}
#endif // MASSEFF
#if HUXLEY
		if (Ar.Game == GAME_Huxley) goto skip_rest_quiet;
#endif
#if DCU_ONLINE
		if (Ar.Game == GAME_DCUniverse && (Ar.ArLicenseeVer & 0xFF00) >= 0x1700) return;
#endif
#if BIOSHOCK3
		if (Ar.Game == GAME_Bioshock3) return;
#endif
		if (Ar.ArVer >= 567)
		{
		tfc_guid:
			Ar << TextureFileCacheGuid;
		}
#if XCOM_BUREAU
		if (Ar.Game == GAME_XcomB) return;
#endif
		// Extra check for some incorrectly upgrated UE3 versions, in particular for
		// Dungeons & Dragons: Daggerdale
		if (Ar.Tell() == Ar.GetStopper()) return;

		if (Ar.ArVer >= 674)
		{
			TArray<FTexture2DMipMap> CachedPVRTCMips;
			Ar << CachedPVRTCMips;
			if (CachedPVRTCMips.Num()) appPrintf("*** %s has %d PVRTC mips (%d normal mips)\n", Name, CachedPVRTCMips.Num(), Mips.Num()); //!!
		}

		if (Ar.ArVer >= 857)
		{
			int CachedFlashMipsMaxResolution;
			TArray<FTexture2DMipMap> CachedATITCMips;
			FByteBulkData CachedFlashMips;
			Ar << CachedFlashMipsMaxResolution;
			Ar << CachedATITCMips;
			CachedFlashMips.Serialize(Ar);
			if (CachedATITCMips.Num()) appPrintf("*** %s has %d ATITC mips (%d normal mips)\n", Name, CachedATITCMips.Num(), Mips.Num()); //!!
		}

#if PLA
		if (Ar.Game == GAME_PLA) goto skip_rest_quiet;
#endif
		if (Ar.ArVer >= 864) Ar << CachedETCMips;

		// some hack to support more games ...
		if (Ar.Tell() < Ar.GetStopper())
		{
			appPrintf("UTexture2D %s: dropping %d bytes\n", Name, Ar.GetStopper() - Ar.Tell());
		skip_rest_quiet:
			DROP_REMAINING_DATA(Ar);
		}

		unguard;
	}

	bool LoadBulkTexture(const TArray<FTexture2DMipMap> &MipsArray, int MipIndex, bool UseETC_TFC) const;
	virtual bool GetTextureData(CTextureData &TexData) const;
#if RENDERING
	virtual void Bind();
	virtual void GetParams(CMaterialParams &Params) const;
	virtual void Release();
#endif
};


class ULightMapTexture2D : public UTexture2D
{
	DECLARE_CLASS(ULightMapTexture2D, UTexture2D)
public:
	virtual void Serialize(FArchive &Ar)
	{
		Super::Serialize(Ar);
		DROP_REMAINING_DATA(Ar);
	}
};


class UTextureCube : public UTexture3
{
	DECLARE_CLASS(UTextureCube, UTexture3)
public:
	UTexture2D		*FacePosX;
	UTexture2D		*FaceNegX;
	UTexture2D		*FacePosY;
	UTexture2D		*FaceNegY;
	UTexture2D		*FacePosZ;
	UTexture2D		*FaceNegZ;

#if RENDERING
	// rendering implementation fields
	GLuint			TexNum;
#endif

	BEGIN_PROP_TABLE
		PROP_OBJ(FacePosX)
		PROP_OBJ(FaceNegX)
		PROP_OBJ(FacePosY)
		PROP_OBJ(FaceNegY)
		PROP_OBJ(FacePosZ)
		PROP_OBJ(FaceNegZ)
	END_PROP_TABLE

	virtual void Serialize(FArchive &Ar)
	{
		Super::Serialize(Ar);
#if TRANSFORMERS
		if (Ar.Game == GAME_Transformers && Ar.ArLicenseeVer >= 108) goto skip_rest_quiet; // has TArray<FTexture2DMipMap>
#endif

		// some hack to support more games ...
		if (Ar.Tell() < Ar.GetStopper())
		{
			appPrintf("UTextureCube %s: dropping %d bytes\n", Name, Ar.GetStopper() - Ar.Tell());
		skip_rest_quiet:
			DROP_REMAINING_DATA(Ar);
		}

	}

#if RENDERING
	virtual void Bind();
	virtual void GetParams(CMaterialParams &Params) const;
	virtual void Release();
	virtual bool IsTextureCube() const
	{
		return true;
	}
#endif // RENDERING
};


enum EBlendMode
{
	BLEND_Opaque,
	BLEND_Masked,
	BLEND_Translucent,
	BLEND_Additive,
	BLEND_Modulate,
	BLEND_ModulateAndAdd,
	BLEND_SoftMasked,
	BLEND_AlphaComposite,
	BLEND_DitheredTranslucent
};

_ENUM(EBlendMode)
{
	_E(BLEND_Opaque),
	_E(BLEND_Masked),
	_E(BLEND_Translucent),
	_E(BLEND_Additive),
	_E(BLEND_Modulate),
	_E(BLEND_ModulateAndAdd),
	_E(BLEND_SoftMasked),
	_E(BLEND_AlphaComposite),
	_E(BLEND_DitheredTranslucent)
};

enum EMaterialLightingModel	// unused now
{
	MLM_Phong,
	MLM_NonDirectional,
	MLM_Unlit,
	MLM_SHPRT,
	MLM_Custom
};

enum EMobileSpecularMask
{
	MSM_Constant,
	MSM_Luminance,
	MSM_DiffuseRed,
	MSM_DiffuseGreen,
	MSM_DiffuseBlue,
	MSM_DiffuseAlpha,
	MSM_MaskTextureRGB,
	MSM_MaskTextureRed,
	MSM_MaskTextureGreen,
	MSM_MaskTextureBlue,
	MSM_MaskTextureAlpha
};

_ENUM(EMobileSpecularMask)
{
	_E(MSM_Constant),
	_E(MSM_Luminance),
	_E(MSM_DiffuseRed),
	_E(MSM_DiffuseGreen),
	_E(MSM_DiffuseBlue),
	_E(MSM_DiffuseAlpha),
	_E(MSM_MaskTextureRGB),
	_E(MSM_MaskTextureRed),
	_E(MSM_MaskTextureGreen),
	_E(MSM_MaskTextureBlue),
	_E(MSM_MaskTextureAlpha)
};


class UMaterialInterface : public UUnrealMaterial
{
	DECLARE_CLASS(UMaterialInterface, UUnrealMaterial)
public:
	// Mobile
	UTexture3		*FlattenedTexture;
	UTexture3		*MobileBaseTexture;
	UTexture3		*MobileNormalTexture;
	bool			bUseMobileSpecular;
	float			MobileSpecularPower;
	EMobileSpecularMask MobileSpecularMask;
	UTexture3		*MobileMaskTexture;

	UMaterialInterface()
	:	MobileSpecularPower(16)
//	,	MobileSpecularMask(MSM_Constant)
	{}

	BEGIN_PROP_TABLE
		PROP_DROP(PreviewMesh)
		PROP_DROP(LightingGuid)
		// Mobile
		PROP_OBJ(FlattenedTexture)
		PROP_OBJ(MobileBaseTexture)
		PROP_OBJ(MobileNormalTexture)
		PROP_DROP(bMobileAllowFog)
		PROP_BOOL(bUseMobileSpecular)
		PROP_DROP(MobileSpecularColor)
		PROP_DROP(bUseMobilePixelSpecular)
		PROP_FLOAT(MobileSpecularPower)
		PROP_ENUM2(MobileSpecularMask, EMobileSpecularMask)
		PROP_OBJ(MobileMaskTexture)
#if MASSEFF
		PROP_DROP(m_Guid)
#endif
	END_PROP_TABLE

#if RENDERING
	virtual void GetParams(CMaterialParams &Params) const;
#endif
};


class UMaterial3 : public UMaterialInterface
{
	DECLARE_CLASS(UMaterial3, UMaterialInterface)
public:
	bool			TwoSided;
	bool			bDisableDepthTest;
	bool			bIsMasked;
	EBlendMode		BlendMode;
	float			OpacityMaskClipValue;
	TArray<UTexture3*> ReferencedTextures;

	UMaterial3()
	:	OpacityMaskClipValue(0.333f)		//?? check
	,	BlendMode(BLEND_Opaque)
	{}

	BEGIN_PROP_TABLE
		PROP_BOOL(TwoSided)
		PROP_BOOL(bDisableDepthTest)
		PROP_BOOL(bIsMasked)
		PROP_ARRAY(ReferencedTextures, UObject*)
		PROP_ENUM2(BlendMode, EBlendMode)
		PROP_FLOAT(OpacityMaskClipValue)
		//!! should be used (main material inputs in UE3 material editor)
		PROP_DROP(DiffuseColor)
		PROP_DROP(DiffusePower)				// GoW2
		PROP_DROP(EmissiveColor)
		PROP_DROP(SpecularColor)
		PROP_DROP(SpecularPower)
		PROP_DROP(Opacity)
		PROP_DROP(OpacityMask)
		PROP_DROP(Distortion)
		PROP_DROP(TwoSidedLightingMask)		// TransmissionMask ?
		PROP_DROP(TwoSidedLightingColor)	// TransmissionColor ?
		PROP_DROP(Normal)
		PROP_DROP(CustomLighting)
		// drop other props
		PROP_DROP(PhysMaterial)
		PROP_DROP(PhysicalMaterial)
		PROP_DROP(LightingModel)			//!! use it (EMaterialLightingModel)
		// usage
		PROP_DROP(bUsedAsLightFunction)
		PROP_DROP(bUsedWithFogVolumes)
		PROP_DROP(bUsedAsSpecialEngineMaterial)
		PROP_DROP(bUsedWithSkeletalMesh)
		PROP_DROP(bUsedWithParticleSystem)
		PROP_DROP(bUsedWithParticleSprites)
		PROP_DROP(bUsedWithBeamTrails)
		PROP_DROP(bUsedWithParticleSubUV)
		PROP_DROP(bUsedWithFoliage)
		PROP_DROP(bUsedWithSpeedTree)
		PROP_DROP(bUsedWithStaticLighting)
		PROP_DROP(bUsedWithLensFlare)
		PROP_DROP(bUsedWithGammaCorrection)
		PROP_DROP(bUsedWithInstancedMeshParticles)
		PROP_DROP(bUsedWithDecals)			// GoW2
		PROP_DROP(bUsedWithFracturedMeshes)	// GoW2
		PROP_DROP(bUsedWithFluidSurfaces)	// GoW2
		PROP_DROP(bUsedWithSplineMeshes)
		// physics
		PROP_DROP(PhysMaterialMask)
		PROP_DROP(PhysMaterialMaskUVChannel)
		PROP_DROP(BlackPhysicalMaterial)
		PROP_DROP(WhitePhysicalMaterial)
		// other
		PROP_DROP(Wireframe)
		PROP_DROP(bIsFallbackMaterial)
		PROP_DROP(FallbackMaterial)			//!! use it
		PROP_DROP(EditorX)
		PROP_DROP(EditorY)
		PROP_DROP(EditorPitch)
		PROP_DROP(EditorYaw)
		PROP_DROP(Expressions)				//!! use it
		PROP_DROP(EditorComments)
		PROP_DROP(EditorCompounds)
		PROP_DROP(bUsesDistortion)
		PROP_DROP(bUsesSceneColor)
		PROP_DROP(bUsedWithMorphTargets)
		PROP_DROP(bAllowFog)
		PROP_DROP(ReferencedTextureGuids)
#if MEDGE
		PROP_DROP(BakerBleedBounceAmount)
		PROP_DROP(BakerAlpha)
#endif // MEDGE
#if TLR
		PROP_DROP(VFXShaderType)
#endif
#if MASSEFF
		PROP_DROP(bUsedWithLightEnvironments)
		PROP_DROP(AllowsEffectsMaterials)
#endif // MASSEFF
	END_PROP_TABLE

	virtual void Serialize(FArchive &Ar)
	{
		Super::Serialize(Ar);
#if BIOSHOCK3
		if (Ar.Game == GAME_Bioshock3)
		{
			// Bioshock Infinite has different internal material format
			FGuid unk44;
			TArray<UTexture2D*> Textures[5];
			Ar << unk44;
			for (int i = 0; i < 5; i++)
			{
				Ar << Textures[i];
				if (!ReferencedTextures.Num() && Textures[i].Num())
					CopyArray(ReferencedTextures, Textures[i]);
	#if 0
				appPrintf("Arr[%d]\n", i);
				for (int j = 0; j < Textures[i].Num(); j++)
				{
					UObject *obj = Textures[i][j];
					appPrintf("... %d: %s %s\n", j, obj ? obj->GetClassName() : "??", obj ? obj->Name : "??");
				}
	#endif
			}
			return;
		}
#endif // BIOSHOCK3
		if (Ar.ArVer >= 858)
		{
			int unkMask;		// default 1
			Ar << unkMask;
		}
		if (Ar.ArVer >= 656)
		{
			guard(SerializeFMaterialResource);
			// Starting with version 656 UE3 has deprecated ReferencedTextures array.
			// This array is serialized inside FMaterialResource which is not needed
			// for us in other case.
			// FMaterialResource serialization is below
			TArray<FString>			f10;
			TMap<UObject*, int>		f1C;
			int						f58;
			FGuid					f60;
			int						f80;
			Ar << f10 << f1C << f58 << f60 << f80;
			if (Ar.ArVer >= 656) Ar << ReferencedTextures;	// that is ...
			// other fields are not interesting ...
			unguard;
		}
		DROP_REMAINING_DATA(Ar);			//?? drop native data
	}

#if RENDERING
	virtual void SetupGL();
	virtual void GetParams(CMaterialParams &Params) const;
	virtual bool IsTranslucent() const;
#endif
};

class UMaterialInstance : public UMaterialInterface
{
	DECLARE_CLASS(UMaterialInstance, UMaterialInterface)
public:
	UUnrealMaterial	*Parent;				// UMaterialInterface*

	BEGIN_PROP_TABLE
		PROP_OBJ(Parent)
		PROP_DROP(PhysMaterial)
		PROP_DROP(bHasStaticPermutationResource)
		PROP_DROP(ReferencedTextures)		// this is a textures from Parent plus own overrided textures
		PROP_DROP(ReferencedTextureGuids)
		PROP_DROP(ParentLightingGuid)
		// physics
		PROP_DROP(PhysMaterialMask)
		PROP_DROP(PhysMaterialMaskUVChannel)
		PROP_DROP(BlackPhysicalMaterial)
		PROP_DROP(WhitePhysicalMaterial)
	END_PROP_TABLE

	virtual void Serialize(FArchive &Ar)
	{
		Super::Serialize(Ar);
		DROP_REMAINING_DATA(Ar);			//?? drop native data
	}
};


struct FScalarParameterValue
{
	DECLARE_STRUCT(FScalarParameterValue)

	FName			ParameterName;
	float			ParameterValue;
//	FGuid			ExpressionGUID;

	BEGIN_PROP_TABLE
		PROP_NAME(ParameterName)
		PROP_FLOAT(ParameterValue)
		PROP_DROP(ExpressionGUID)	//!! test nested structure serialization later
#if TRANSFORMERS
		PROP_DROP(ParameterCategory)
#endif
	END_PROP_TABLE
};

struct FTextureParameterValue
{
	DECLARE_STRUCT(FTextureParameterValue)

	FName			ParameterName;
	UTexture3		*ParameterValue;
//	FGuid			ExpressionGUID;

	BEGIN_PROP_TABLE
		PROP_NAME(ParameterName)
		PROP_OBJ(ParameterValue)
		PROP_DROP(ExpressionGUID)	//!! test nested structure serialization later
#if TRANSFORMERS
		PROP_DROP(ParameterCategory)
#endif
	END_PROP_TABLE
};

struct FVectorParameterValue
{
	DECLARE_STRUCT(FVectorParameterValue)

	FName			ParameterName;
	FLinearColor	ParameterValue;
//	FGuid			ExpressionGUID;

	BEGIN_PROP_TABLE
		PROP_NAME(ParameterName)
		PROP_STRUC(ParameterValue, FLinearColor)
		PROP_DROP(ExpressionGUID)	//!! test nested structure serialization later
#if TRANSFORMERS
		PROP_DROP(ParameterCategory)
#endif
	END_PROP_TABLE
};

class UMaterialInstanceConstant : public UMaterialInstance
{
	DECLARE_CLASS(UMaterialInstanceConstant, UMaterialInstance)
public:
	TArray<FScalarParameterValue>	ScalarParameterValues;
	TArray<FTextureParameterValue>	TextureParameterValues;
	TArray<FVectorParameterValue>	VectorParameterValues;

	BEGIN_PROP_TABLE
		PROP_ARRAY(ScalarParameterValues,  FScalarParameterValue )
		PROP_ARRAY(TextureParameterValues, FTextureParameterValue)
		PROP_ARRAY(VectorParameterValues,  FVectorParameterValue )
		PROP_DROP(FontParameterValues)
	END_PROP_TABLE

#if RENDERING
	virtual void SetupGL();
	virtual void GetParams(CMaterialParams &Params) const;
	virtual bool IsTranslucent() const
	{
		return Parent ? Parent->IsTranslucent() : false;
	}
#endif
};


#if DCU_ONLINE

struct UIStreamingTexture_Info_DCU
{
	unsigned		Hash;				// real texture has no hash, it is a Map<int,UIStreamingTexture_Info>
	int				nWidth;
	int				nHeight;
	int				BulkDataFlags;
	int				ElementCount;
	int				BulkDataOffsetInFile;
	int				BulkDataSizeOnDisk;
	int				Format;
	byte			bSRGB;
	FName			TextureFileCacheName;

	friend FArchive& operator<<(FArchive &Ar, UIStreamingTexture_Info_DCU &S)
	{
		Ar << S.Hash;					// serialize it in the structure
		return Ar << S.nWidth << S.nHeight << S.BulkDataFlags << S.ElementCount << S.BulkDataOffsetInFile
				  << S.BulkDataSizeOnDisk << S.Format << S.bSRGB << S.TextureFileCacheName;
	}
};

//?? non-functional class, remove it later
class UUIStreamingTextures : public UObject
{
	DECLARE_CLASS(UUIStreamingTextures, UObject);
public:
	TArray<UIStreamingTexture_Info_DCU> TextureHashToInfo;

	// generated data
	TArray<UTexture2D*>		Textures;
	TArray<char*>			Names;
	virtual ~UUIStreamingTextures();

	virtual void Serialize(FArchive &Ar)
	{
		guard(UUIStreamingTextures::Serialize);
		Super::Serialize(Ar);
		Ar << TextureHashToInfo;
		unguard;
	}

	virtual void PostLoad();
};

#endif // DCU_ONLINE


#define REGISTER_MATERIAL_CLASSES_U3	\
	REGISTER_CLASS_ALIAS(UMaterial3, UMaterial) \
	REGISTER_CLASS(UTexture2D)			\
	REGISTER_CLASS(ULightMapTexture2D)	\
	REGISTER_CLASS(UTextureCube)		\
	REGISTER_CLASS(FScalarParameterValue)  \
	REGISTER_CLASS(FTextureParameterValue) \
	REGISTER_CLASS(FVectorParameterValue)  \
	REGISTER_CLASS(UMaterialInstanceConstant)

#define REGISTER_MATERIAL_CLASSES_DCUO	\
	REGISTER_CLASS(UUIStreamingTextures)

#define REGISTER_MATERIAL_ENUMS_U3		\
	REGISTER_ENUM(EPixelFormat)			\
	REGISTER_ENUM(ETextureAddress)		\
	REGISTER_ENUM(EBlendMode)			\
	REGISTER_ENUM(EMobileSpecularMask)


#endif // UNREAL3

#endif // __UNMATERIAL3_H__