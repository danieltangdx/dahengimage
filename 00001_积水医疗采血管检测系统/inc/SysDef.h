/************************************************************************/
/*                       [6/6/2017 ConanSteve]                         */
enum e_ElementIsWriting {
	e_ElementIsWriting_NotWrite,
	e_ElementIsWriting_IsWriting,
	e_ElementIsWriting_Complete
};
enum e_ElementIsDetecting {
	e_ElementIsDetecting_NotDectect,
	e_ElementIsDetecting_Detecting
};

#define e_AlgStatus_Default -1
enum e_AlgStatus {
	e_AlgStatus_Good = 0,
	e_AlgStatus_Bad,
	e_AlgStatus_AlgException,
	e_AlgStatus_Empty,
};

enum e_CamMode {
	e_CamMode_Normal,
	e_CamMode_Test
};

/************************************************************************/