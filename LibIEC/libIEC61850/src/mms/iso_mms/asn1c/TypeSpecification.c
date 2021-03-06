﻿/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#include <asn_internal.h>

#include "TypeSpecification.h"

static asn_TYPE_member_t asn_MBR_array_3[] = {
	{ ATF_POINTER, 1, offsetof(struct TypeSpecification__array, packed),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"packed"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification__array, numberOfElements),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Unsigned32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"numberOfElements"
		},
	{ ATF_POINTER, 0, offsetof(struct TypeSpecification__array, elementType),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_TypeSpecification,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"elementType"
		},
};
static ber_tlv_tag_t asn_DEF_array_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_array_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* packed at 498 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* numberOfElements at 499 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* elementType at 501 */
};
static asn_SEQUENCE_specifics_t asn_SPC_array_specs_3 = {
	sizeof(struct TypeSpecification__array),
	offsetof(struct TypeSpecification__array, _asn_ctx),
	asn_MAP_array_tag2el_3,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_array_3 = {
	"array",
	"array",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_array_tags_3,
	sizeof(asn_DEF_array_tags_3)
		/sizeof(asn_DEF_array_tags_3[0]) - 1, /* 1 */
	asn_DEF_array_tags_3,	/* Same as above */
	sizeof(asn_DEF_array_tags_3)
		/sizeof(asn_DEF_array_tags_3[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_array_3,
	3,	/* Elements count */
	&asn_SPC_array_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_components_9[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_StructComponent,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_components_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_components_specs_9 = {
	sizeof(struct TypeSpecification__structure__components),
	offsetof(struct TypeSpecification__structure__components, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_components_9 = {
	"components",
	"components",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_components_tags_9,
	sizeof(asn_DEF_components_tags_9)
		/sizeof(asn_DEF_components_tags_9[0]) - 1, /* 1 */
	asn_DEF_components_tags_9,	/* Same as above */
	sizeof(asn_DEF_components_tags_9)
		/sizeof(asn_DEF_components_tags_9[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_components_9,
	1,	/* Single element */
	&asn_SPC_components_specs_9	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_structure_7[] = {
	{ ATF_POINTER, 1, offsetof(struct TypeSpecification__structure, packed),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"packed"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification__structure, components),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_components_9,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"components"
		},
};
static ber_tlv_tag_t asn_DEF_structure_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_structure_tag2el_7[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* packed at 504 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* components at 506 */
};
static asn_SEQUENCE_specifics_t asn_SPC_structure_specs_7 = {
	sizeof(struct TypeSpecification__structure),
	offsetof(struct TypeSpecification__structure, _asn_ctx),
	asn_MAP_structure_tag2el_7,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_structure_7 = {
	"structure",
	"structure",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_structure_tags_7,
	sizeof(asn_DEF_structure_tags_7)
		/sizeof(asn_DEF_structure_tags_7[0]) - 1, /* 1 */
	asn_DEF_structure_tags_7,	/* Same as above */
	sizeof(asn_DEF_structure_tags_7)
		/sizeof(asn_DEF_structure_tags_7[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_structure_7,
	2,	/* Elements count */
	&asn_SPC_structure_specs_7	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_floatingpoint_15[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification__floatingpoint, formatwidth),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_Unsigned8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"formatwidth"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification__floatingpoint, exponentwidth),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_Unsigned8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"exponentwidth"
		},
};
static ber_tlv_tag_t asn_DEF_floatingpoint_tags_15[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_floatingpoint_tag2el_15[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 1 }, /* formatwidth at 514 */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, -1, 0 } /* exponentwidth at 520 */
};
static asn_SEQUENCE_specifics_t asn_SPC_floatingpoint_specs_15 = {
	sizeof(struct TypeSpecification__floatingpoint),
	offsetof(struct TypeSpecification__floatingpoint, _asn_ctx),
	asn_MAP_floatingpoint_tag2el_15,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_floatingpoint_15 = {
	"floatingpoint",
	"floatingpoint",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_floatingpoint_tags_15,
	sizeof(asn_DEF_floatingpoint_tags_15)
		/sizeof(asn_DEF_floatingpoint_tags_15[0]) - 1, /* 1 */
	asn_DEF_floatingpoint_tags_15,	/* Same as above */
	sizeof(asn_DEF_floatingpoint_tags_15)
		/sizeof(asn_DEF_floatingpoint_tags_15[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_floatingpoint_15,
	2,	/* Elements count */
	&asn_SPC_floatingpoint_specs_15	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_TypeSpecification_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.typeName),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ObjectName,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"typeName"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.array),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_array_3,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"array"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.structure),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_structure_7,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"structure"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.boolean),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"boolean"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.bitstring),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bitstring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.integer),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Unsigned8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"integer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.Unsigned),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Unsigned8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"unsigned"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.floatingpoint),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_floatingpoint_15,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"floatingpoint"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.octetstring),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"octetstring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.visiblestring),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"visiblestring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.generalizedtime),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"generalizedtime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.binarytime),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"binarytime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.bcd),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Unsigned8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bcd"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.objId),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"objId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.mMSString),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mMSString"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TypeSpecification, choice.utctime),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"utctime"
		},
};
static asn_TYPE_tag2member_t asn_MAP_TypeSpecification_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* typeName at 495 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* array at 498 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* structure at 504 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* boolean at 509 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* bitstring at 510 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* integer at 511 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* unsigned at 512 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* floatingpoint at 514 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 8, 0, 0 }, /* octetstring at 521 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 9, 0, 0 }, /* visiblestring at 522 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 10, 0, 0 }, /* generalizedtime at 523 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 11, 0, 0 }, /* binarytime at 524 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 12, 0, 0 }, /* bcd at 525 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 13, 0, 0 }, /* objId at 526 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 14, 0, 0 }, /* mMSString at 527 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 15, 0, 0 } /* utctime at 528 */
};
static asn_CHOICE_specifics_t asn_SPC_TypeSpecification_specs_1 = {
	sizeof(struct TypeSpecification),
	offsetof(struct TypeSpecification, _asn_ctx),
	offsetof(struct TypeSpecification, present),
	sizeof(((struct TypeSpecification *)0)->present),
	asn_MAP_TypeSpecification_tag2el_1,
	16,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_TypeSpecification = {
	"TypeSpecification",
	"TypeSpecification",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_TypeSpecification_1,
	16,	/* Elements count */
	&asn_SPC_TypeSpecification_specs_1	/* Additional specs */
};

