#include <stdint.h>

#ifndef SDM_H_INCLUDE
#define SDM_H_INCLUDE 
#if 0
Information collected from
Intel 64 and IA-32 Architectures Software Developer's Manual
Volume 4: Model-Specific Registers


df_dm   Volume 4 tables
-------------------------------------------------------------------
06_2AH	2-20 2-21 2-22
06_2DH	2-20 2-23 2-24
06_3AH	2-20 2-21 2-22 2-25
06_3CH	2-20 2-21 2-22 2-29 2-30
06_3DH	2-20 2-21 2-22 2-25 2-29 2-30 2-34 2-35
06_3EH	2-20 2-24 2-26 2-27 2-28
06_3FH	2-20 2-29 2-32 2-33
06_45H	2-20 2-21 2-22 2-29 2-30 2-31 2-38
06_46H	2-20 2-21 2-22 2-29 2-30
06_47H	2-20 2-21 2-22 2-25 2-29 2-30 2-34 2-35
06_4EH	2-20 2-21 2-25 2-29 2-35 2-39 2-40
06_4FH	2-20 2-21 2-29 2-34 2-36 2-37 2-38
06_55H	2-20 2-21 2-25 2-29 2-35 2-39 2-50
06_56H	2-20 2-29 2-34 2-36 2-37
06_5EH	2-20 2-21 2-25 2-29 2-35 2-39 2-40
06_66H	2-20 2-21 2-25 2-29 2-35 2-39 2-40 2-42 2-43
06_6AH	2-20 2-21 2-25 2-29 2-35 2-39 2-51
06_6CH	2-20 2-21 2-25 2-29 2-35 2-39
06_7DH	2-20 2-21 2-25 2-29 2-35 2-39 2-44
06_7EH	2-20 2-21 2-25 2-29 2-35 2-39 2-44
06_8CH	2-20 2-21 2-25 2-29 2-35 2-39 2-45
06_8DH	2-20 2-21 2-25 2-29 2-35 2-39 2-45
06_8EH	2-20 2-21 2-25 2-29 2-35 2-39 2-40
06_8FH	2-52
06_97H	2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49
06_9AH	2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49
06_9EH	2-20 2-21 2-25 2-29 2-35 2-39 2-40 2-41
06_A5H	2-20 2-21 2-25 2-29 2-35 2-39
06_A6H	2-20 2-21 2-25 2-29 2-35 2-39
06_BFH	2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49

table	df_dm
=========================================================================================================================================================================================================================
2-20	06_2AH 06_2DH 06_3AH 06_3CH 06_3DH 06_3EH 06_3FH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_56H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-21	06_2AH 06_3AH 06_3CH 06_3DH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-22	06_2AH 06_3AH 06_3CH 06_3DH 06_45H 06_46H 06_47H
2-23	06_2DH
2-24	06_2DH 06_3EH
2-25	06_3AH 06_3DH 06_47H 06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-26	06_3EH
2-27	06_3EH
2-28	06_3EH
2-29	06_3CH 06_3DH 06_3FH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_56H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-30	06_3CH 06_3DH 06_45H 06_46H 06_47H
2-31	06_45H
2-32	06_3FH
2-33	06_3FH
2-34	06_3DH 06_47H 06_4FH 06_56H
2-35	06_3DH 06_47H 06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-36	06_4FH 06_56H
2-37	06_4FH 06_56H
2-38	06_45H 06_4FH
2-39	06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH
2-40	06_4EH 06_5EH 06_66H 06_8EH 06_9EH
2-41	06_9EH
2-42	06_66H
2-43	06_66H
2-44	06_7DH 06_7EH 06_97H 06_9AH 06_BFH
2-45	06_8CH 06_8DH 06_97H 06_9AH 06_BFH
2-46	06_97H 06_9AH 06_BFH
2-47	06_97H 06_9AH 06_BFH
2-48	06_97H 06_9AH 06_BFH
2-49	06_97H 06_9AH 06_BFH
2-50	06_55H
2-51	06_6AH
2-52	06_8FH
#endif //0

#ifndef CONFIG_ARCHIECTURE_H_INCLUDE
/// @brief List of Intel family and models.
enum intel_arch_e
{
    FM_06_2A = 0x2A, //Sandy Bridge
    FM_06_2D = 0x2D, //Sandy Bridge
    FM_06_3A = 0x3A, //Ivy Bridge
    FM_06_3C = 0x3C, //Haswell
    FM_06_3D = 0x3D, //Broadwell
    FM_06_3E = 0x3E, //Ivy Bridge
    FM_06_3F = 0x3F, //Haswell
    FM_06_45 = 0x45, //Haswell
    FM_06_46 = 0x46, //Haswell
    FM_06_47 = 0x47, //Broadwell
    FM_06_4E = 0x4E, //Skylake
    FM_06_4F = 0x4F, //Broadwell
    FM_06_55 = 0x55, //Skylake, Cascade Lake, Cooper Lake
    FM_06_56 = 0x56, //Broadwell
    FM_06_5E = 0x5E, //Skylake
    FM_06_66 = 0x66, //Cannon Lake
    FM_06_6A = 0x6A, //Ice Lake
    FM_06_6C = 0x6C, //Ice Lake
    FM_06_7D = 0x7D, //Ice Lake
    FM_06_7E = 0x7E, //Ice Lake
    FM_06_8C = 0x8C, //Tiger Lake
    FM_06_8D = 0x8D, //Tiger Lake
    FM_06_8E = 0x8E, //Coffee Lake
    FM_06_8F = 0x8F, //Sapphire Rapids
    FM_06_97 = 0x97, //Alder Lake
    FM_06_9A = 0x9A, //Alder Lake
    FM_06_9E = 0x9E, //Kaby Lake, Coffee Lake
    FM_06_A5 = 0xA5, //Comet Lake
    FM_06_A6 = 0xA6, //Comet Lake
    FM_06_A7 = 0xA7, //Rocket Lake (No MSRs listed in SDM)
    FM_06_A8 = 0xA8, //Rocket Lake (No MSRs listed in SDM)
    FM_06_BF = 0xBF, //Alder Lake
};
#endif

// Currently lives in config_architecture.h
// All of these models belong to display_family 06.
enum idx_dfdm : uint64_t    // Might need -std=c23 or -std=c2x to get this to compile.
{
    IDX_2A = 0, //Sandy Bridge
    IDX_2D, //Sandy Bridge
    IDX_3A, //Ivy Bridge
    IDX_3C, //Haswell
    IDX_3D, //Broadwell
    IDX_3E, //Ivy Bridge
    IDX_3F, //Haswell
    IDX_45, //Haswell
    IDX_46, //Haswell
    IDX_47, //Broadwell
    IDX_4E, //Skylake
    IDX_4F, //Broadwell
    IDX_55, //Skylake, Cascade Lake, Cooper Lake
    IDX_56, //Broadwell
    IDX_5E, //Skylake
    IDX_66, //Cannon Lake
    IDX_6A, //Ice Lake
    IDX_6C, //Ice Lake
    IDX_7D, //Ice Lake
    IDX_7E, //Ice Lake
    IDX_8C, //Tiger Lake
    IDX_8D, //Tiger Lake
    IDX_8E, //Coffee Lake
    IDX_8F, //Sapphire Rapids
    IDX_97, //Alder Lake
    IDX_9A, //Alder Lake
    IDX_9E, //Kaby Lake, Coffee Lake
    IDX_A5, //Comet Lake
    IDX_A6, //Comet Lake
    IDX_A7, //Rocket Lake (No MSRs listed in SDM)
    IDX_A8, //Rocket Lake (No MSRs listed in SDM)
    IDX_BF, //Alder Lake
    n____x, //Dummy value ("no_idxx")
};

// The table numbers might not be stable in future Intel revisions.
// This enum should provide an additional layer of indirection.
// Table titles provided in comments to help aid navigative should
// things get perturbed.  I've normalized spelling, grammar and
// punctuation and removed trademark and similar symbols.
enum tables : uint64_t
{
    // Table 2-20.  MSRs supported by Intel procesors based on Sandy Bridge
    // microarchitecture.
    T20 = 0x0000'0000'0000'0001,

    // Table 2-21.  MSRs supported by the 2nd generation Intel Core processors
    // (Sandy Bridge Microarchitecture).
    T21 = 0x0000'0000'0000'0002,

    // Table 2-22.  Uncore PMU MSRs supported by 2nd Generate Intel Core
    // processors.
    T22 = 0x0000'0000'0000'0004,

    // Table 2-23.  Selected MSRs supported by Intel Xeon processors E5 family
    // based on Sandy Bridge microarchitecture.
    T23 = 0x0000'0000'0000'0008,

    // Table 2-24.  Uncore PMU MSRs in Intel Xeon processor E5 family.
    T24 = 0x0000'0000'0000'0010,

    // Table 2-25.  Additional MSRs supported by 3rds generation Intel Core
    // processors (based on Ivy Bridge microarchitecture).
    T25 = 0x0000'0000'0000'0020,

    // Table 2-26.  MSRs in Intel Xeon processor E5 v2 product family (based on
    // Ivy Bridge-E microarchitecture).
    T26 = 0x0000'0000'0000'0040,

    // Table 2-27.  Additional MSRs supported by Intel Xeon processor E7 v2
    // family (06_3EH).
    T27 = 0x0000'0000'0000'0080,

    // Table 2-28.  Additional Uncore PMU MSRs in the Intel Xeon processor E5
    // v2 and E7 v2 families
    T28 = 0x0000'0000'0000'0100,

    // Table 2-29.  Additional MSRs supported by processors based on the
    // Haswell or Haswell-E microarchitecture
    T29 = 0x0000'0000'0000'0200,

    // Table 2-30.  MSRs supported by 4th generation Intel Core processors
    // (Haswell microarchitecture)
    T30 = 0x0000'0000'0000'0400,

    // Table 2-31.  Additional residency MSRs supported by 4th generation Intel
    // Core processors (06_45H)
    T31 = 0x0000'0000'0000'0800,

    // Table 2-32.  Additional MSRs supported by Intel Xeon processor E5 v3
    // family
    T32 = 0x0000'0000'0000'1000,

    // Table 2-33.  Uncore PMU MSRs in Intel Xeon processor E5 v3 family
    T33 = 0x0000'0000'0000'2000,

    // Table 2-34.  Additional MSRs common to processors based on the Broadwell
    // Microarchitecture
    T34 = 0x0000'0000'0000'4000,

    // Table 2-35.  Additional MSRs supported by Intel Core M processors and
    // 5th generation Intel Core processors.
    T35 = 0x0000'0000'0000'8000,

    // Table 2-36.  Additional MSRs common to Intel Xeon processor D and Intel
    // Xeon processors E5 v4 family based on the Broadwell microarchitecture
    T36 = 0x0000'0000'0001'0000,

    // Table 2-37.  Additional MSRs supported by Intel Xeon processor D
    // (06_56H)
    T37 = 0x0000'0000'0002'0000,

    // Table 2-38.  Additional MSRs supported by Intel Xeon processors (06_4FH)
    T38 = 0x0000'0000'0004'0000,

    // Table 2-39.  Additional MSRs supported by 6th generation, 7th
    // generation, 8th generation, 9th generation, 10th generation, 11th
    // generation, and 12th generation Intel Core processors, Intel Xeon
    // processor Scalable family, 2nd and 3rd generation Intel Xeon processor
    // Scalable family, 8th generation Intel Core i3 processors and Intel Xeon
    // E processors.
    T39 = 0x0000'0000'0008'0000,

    // Table 2-40.  Uncore PMU MSRs supported by 6th generation, 7th
    // generation, and 8th generation Intel Core processors, and 8th generation
    // Intel Core i3 processors.
    T40 = 0x0000'0000'0010'0000,

    // Table 2-41.  Additional MSRs supported by 7th generation and 8th
    // generation Intel Core processors based on Kaby Lake microarchitecture
    // and Coffee Lake microarchitecture.
    T41 = 0x0000'0000'0020'0000,

    // Table 2-42.  Additional MSRs supported by 8th generation Intel COre i3
    // processors based on Cannon Lake microarchitecture.
    T42 = 0x0000'0000'0040'0000,

    // Table 2-43.  Uncore PMU MSRs supported by Intel Core processors based on
    // Cannon Lake microarchitecture.
    T43 = 0x0000'0000'0080'0000,

    // Table 2-44.  MSRs supported by 10th generation Intel Core processors
    // based on Ice Lake microarchitecture.
    T44 = 0x0000'0000'0100'0000,

    // Table 2-45.  Additional MSRs supported by 11th generation Intel Core
    // processors based on Tiger Lake microarchitecture.
    T45 = 0x0000'0000'0200'0000,

    // Table 2-46.  Additional MSRs supported by 12th generation Intel Core
    // processors supporting performance hybrid architecture
    T46 = 0x0000'0000'0400'0000,

    // Table 2-47.  MSRs supported by 12th generation Intel Core processor
    // P-core.
    T47 = 0x0000'0000'0800'0000,

    // Table 2-48.  MSRs supported by 12th generation Intel Core processor
    // E-core.
    T48 = 0x0000'0000'1000'0000,

    // Table 2-49.  Uncore PMU MSRs supported by 12th generation Intel Core
    // processors.
    T49 = 0x0000'0000'2000'0000,

    // Table 2-50.  MSRs supported by Intel Xeon processor Scalable family
    // (06_55H)
    T50 = 0x0000'0000'4000'0000,

    // Table 2-51.  MSRs specific to 3rd generation Intel Xeon processor
    // Scalable Family based on Ice Lake microarchitecture.

    // Table 2-52.  Additional MSRs supported by future Intel Xeon procesors
    // (06_8F)
    T51 = 0x0000'0000'8000'0000,
};

// The MSR catalog.
uint64_t cat[] = {
    /* 06_2AH */    T20 | T21 | T22,
	/* 06_2DH */	T20 | T23 | T24,
	/* 06_3AH */	T20 | T21 | T22 | T25,
	/* 06_3CH */	T20 | T21 | T22 | T29 | T30,
	/* 06_3DH */	T20 | T21 | T22 | T25 | T29 | T30 | T34 | T35,
	/* 06_3EH */	T20 | T24 | T26 | T27 | T28,
	/* 06_3FH */	T20 | T29 | T32 | T33,
	/* 06_45H */	T20 | T21 | T22 | T29 | T30 | T31 | T38,
	/* 06_46H */	T20 | T21 | T22 | T29 | T30,
	/* 06_47H */	T20 | T21 | T22 | T25 | T29 | T30 | T34 | T35,
	/* 06_4EH */	T20 | T21 | T25 | T29 | T35 | T39 | T40,
	/* 06_4FH */	T20 | T21 | T29 | T34 | T36 | T37 | T38,
	/* 06_55H */	T20 | T21 | T25 | T29 | T35 | T39 | T50,
	/* 06_56H */	T20 | T29 | T34 | T36 | T37,
	/* 06_5EH */	T20 | T21 | T25 | T29 | T35 | T39 | T40,
	/* 06_66H */	T20 | T21 | T25 | T29 | T35 | T39 | T40 | T42 | T43,
	/* 06_6AH */	T20 | T21 | T25 | T29 | T35 | T39 | T51,
	/* 06_6CH */	T20 | T21 | T25 | T29 | T35 | T39,
	/* 06_7DH */	T20 | T21 | T25 | T29 | T35 | T39 | T44,
	/* 06_7EH */	T20 | T21 | T25 | T29 | T35 | T39 | T44,
	/* 06_8CH */	T20 | T21 | T25 | T29 | T35 | T39 | T45,
	/* 06_8DH */	T20 | T21 | T25 | T29 | T35 | T39 | T45,
	/* 06_8EH */	T20 | T21 | T25 | T29 | T35 | T39 | T40,
	/* 06_8FH */	T52,
	/* 06_97H */	T20 | T21 | T25 | T29 | T35 | T39 | T46 | T44 | T45 | T47 | T48 | T49,
	/* 06_9AH */	T20 | T21 | T25 | T29 | T35 | T39 | T46 | T44 | T45 | T47 | T48 | T49,
	/* 06_9EH */	T20 | T21 | T25 | T29 | T35 | T39 | T40 | T41,
	/* 06_A5H */	T20 | T21 | T25 | T29 | T35 | T39,
	/* 06_A6H */	T20 | T21 | T25 | T29 | T35 | T39,
	/* 06_BFH */	T20 | T21 | T25 | T29 | T35 | T39 | T46 | T44 | T45 | T47 | T48 | T49
};


#endif //SDM_H_INCLUDE
