#pragma once

// 주요 오브젝트 관련. (플레이어, 씬, 몬스터 ..)
enum OBJECT_TYPE{
	OBJ_FIRSTFLOOR, OBJ_SECONDFLOOR, OBJ_THIRDFLOOR, OBJ_FOURTHFLOOR, OBJ_FIFTHFLOOR, 
	OBJ_PLAYER,
	OBJ_NPC,
	OBJ_ROPE,
	OBJ_ARROW, OBJ_SKILL_WING, OBJ_SKILL_SHOOT, OBJ_SKILL_SWING, OBJ_SKILL_DRAGON, OBJ_SKILL_FIRE,
	OBJ_MONSTER, OBJ_GRRENMUSH, OBJ_COUPLEMUSH,
	OBJ_ITEM,
	OBJ_EFFECT,
	OBJ_UI, OBJ_HPBAR,
	OBJ_MOUSE,
	OBJ_MYFONT,
	OBJ_END
};

enum OBJECT_DIR {
	DIR_LEFT, DIR_RIGHT, DIR_ROPE, DIR_END
};

enum SCENE_TYPE {
	SCENE_LOGO, SCENE_MENU, SCENE_EDIT,
	SCENE_MAKINGPLAYER,
	SCENE_FIELD, SCENE_STAGE1, 
	SCENE_END
};

enum PLAYER_STATE {
	PLAYER_STAND, PLAYER_WALK, PLAYER_JUMP, PLAYER_ROPE, 
	PLAYER_ATTACK, PLAYER_SWING,
	PLAYER_CRAWLATTACK, PLAYER_CRAWL,
	PLAYER_SHOOT, PLAYER_DAMAGED
};

enum MONSTER_STATE {
	MONSTER_STAND, MONSTER_WALK,/* MONSTER_JUMP, MONSTER_FLY,*/
	MONSTER_ATTACK1, MONSTER_ATTACK2, MONSTER_SKILL1, MONSTER_SKILL2, MONSTER_SKILL3, MONSTER_SKILL4, MONSTER_SKILL5,
	MONSTER_DAMAGED, MONSTER_DEAD
};

enum MOUSE_STATE { MOUSE_BASIC, MOUSE_GRAB };



enum GOLD_ID { GOLD_0, GOLD_1, GOLD_2, GOLD_3 };

enum ARROW_ID { ARROW_BASIC, ARROW_DRAGON };

// 그 외 오브젝트. (지형, 충돌, 폰트 등..)

enum RENDER_TYPE { RENDER_BACKGROUND, RENDER_OBJ, RENDER_EFFECT, RENDER_UI, RENDER_END };

enum LINE_ID { LINE_FIELD, LINE_STAGE1, LINE_STAGE2, LINE_BOSS, LINE_END };

enum COLLISION_ID {
	COL_PLAYER_ROPE,
	COL_PLAYER_FIRSTFLOOR, COL_PLAYER_SECONDFLOOR, COL_PLAYER_THIRDFLOOR, COL_PLAYER_FOURTHFLOOR, COL_PLAYER_FIFTHFLOOR,
	COL_MONSTER_FIRSTFLOOR, COL_MONSTER_SECONDFLOOR, COL_MONSTER_THIRDFLOOR, COL_MONSTER_FOURTHFLOOR, COL_MONSTER_FIFTHFLOOR,
	COL_MONSTER_SKILL, COL_MONSTER_ARROW, COL_MONSTER_SWING, COL_MONSTER_EFFECT,

	COL_PLAYER_MONSTER,
	COL_PLAYER_BOSS,

	COL_PLAYER_ITEM,
	COL_END
};

enum FLOORBOX_ID { FLOORBOX_HEIGHT, FLOORBOX_WIDTH, FLOORBOX_END };


enum FONT_ID { FONTID_DAMAGED, FONTID_ATTACK, FONTID_CRITICAL };

// Player
enum PLAYER_JOB { JOB_STRIKER, JOB_CAPTIN, JOB_END };

// Skill
enum SKILL_TYPE { SKILL_SHOOT, SKILL_FIRE, SKILL_END };

//--------------------------------------------------------------
// 서버 --------------------------------------------------------
// 충돌 체크 관련해서 폰트 띄울 때.
enum FONT_TYPE { FONTTYPE_UNIT, FONTTYPE_TENS, FONTTYPE_HUND, FONTTYPE_THOU, FONTTYPE_TENTH };
enum FONT_FLOOR { FONT_FIRSTFLOOR, FONT_SECONDFLOOR, FONT_THIRDFLOOR, FONT_FOURTHFLOOR, FONT_FIFTHFLOOR };

enum PACKET_TYPE { // CS - Client to Server. SC - Server to Client.
	CS_PACKET_PLAYERINFO_INITIALLY,		// 클라이언트 : 내가 새로 접속했어. 나는 닉네임과 직업을 정했고, 이걸 playerinfo에 채워서 보낼게. // 서버 : 닉네임과 직업? 잘 알았어. 니 아이디를 부여하기 위한 작업을 할게.
	CS_PACKET_PLAYERINFO_MOVE, // 클라이언트 : 나 움직였다! 내 playerinfo 보내 줄게. // 서버 : g_vecplayer[니 id]에 갱신할게. 다른 플레이어에게도 니 정보를 전송할게.
	CS_PACKET_PLAYERINFO_INCHANGINGSCENE,// 클라이언트 : 나 씬 바꾼다! 바뀐 내 좌표 너한테 보내줄게. // 서버 : g_vecplayer[니 id]에 갱신할게. 다른 플레이어한테도 니 정보를 전송할게 그럼.
	CS_PACKET_SKILL_CREATE,	// 클라이언트 : 나 스킬 썼어! 이 Skillinfo를 보내줄게. // 서버 : 그럼 그 스킬에 id를 부여할게. 그리고 g_vecSkill[id]에 갱신할게. 그리고 다른 클라이언트에게도 이 스킬 정보를 보낼게.
	CS_PACKET_SKILL_MOVE,

	CS_PACKET_GRRENMUSH,
	//----------------------------------------
	SC_PACKET_YOUR_PLAYERINFO, // 서버 : 너 상태 바뀌었지? or 새로 접속했지? 니 playerinfo는 이거야. // 오 알았어! 내 g_vecplayer[내 아이디]에 저장할게.
	SC_PACKET_OTHER_PLAYERINFO, 	// 서버 : 다른 클라이언트가 상태가 바뀌었어. 이 아이의 info는 이래. // 클라이언트 : 알았어. 나의 g_vecplayer[상태 바뀐 클라의 id]에 갱신할게.
	SC_PACKET_NEW_OTHER_PLAYERINFO, // 서버:  새로운 다른 클라이언트가 들어왔어. 이 아이의 info는 이래. // 클라이언트 : 알았어. CreateObj를 해서 걔의 정보를 넣고, 나의 g_vecplayer[상태 바뀐 클라의 id]에 갱신할게.
	SC_PACKET_SKILL_CREATE, // 서버 : 어떤 클라이언트가 이런 스킬을 생성했어. 정보를 보낼게. // 클라이언트 : 그러면 그 스킬을 새로 만들게.
	SC_PACKET_SKILL_MOVE,

	SC_PACKET_CLIENT_END,	// 클라이언트 종료 서버한테 알리기
	SC_PACKET_GRRENMUSH
};