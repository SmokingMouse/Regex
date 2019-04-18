
extern const char g_escape_table[128] = {
	0   ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,
	8   ,9   ,10  ,11  ,12  ,13  ,14  ,15  ,
	16  ,17  ,18  ,19  ,20  ,21  ,22  ,23  ,
	24  ,25  ,26  ,27  ,28  ,29  ,30  ,31  ,
	32  ,33  ,34  ,35  ,36  ,37  ,38  ,39  ,
	-41 ,-42 ,-43 ,-44 ,-45 ,-46 ,-47 ,47  ,
	48  ,49  ,50  ,51  ,52  ,53  ,54  ,55  ,
	56  ,57  ,58  ,59  ,60  ,61  ,62  ,-64 ,
	64  ,65  ,66  ,67  ,68  ,69  ,70  ,71  ,
	72  ,73  ,74  ,75  ,76  ,77  ,78  ,79  ,
	80  ,81  ,82  ,83  ,84  ,85  ,86  ,87  ,
	88  ,89  ,90  ,-92 ,92  ,-94 ,-95 ,95  ,
	96  ,97  ,98  ,99  ,100 ,101 ,102 ,103 ,
	104 ,105 ,106 ,107 ,108 ,109 ,110 ,111 ,
	112 ,113 ,114 ,115 ,116 ,117 ,118 ,119 ,
	120 ,121 ,122 ,-124,-125,-126,126 ,127
};//某个字符的含义


extern const char g_ctl_left_paren = -41;		// (
extern const char g_ctl_right_paren = -42;		// )
extern const char g_ctl_star = -43;				// *
extern const char g_ctl_plus = -44;				// +
extern const char g_ctl_comma = -45;			// ,
extern const char g_ctl_minus = -46;			// -
extern const char g_ctl_point = -47;			// .
extern const char g_ctl_quesmark = -64;			// ?
extern const char g_ctl_left_bracket = -92;		// [
//const extern char g_ctl_escape = -93;			// \  在经过预处理后，剩余的\都是非功能符。 
extern const char g_ctl_right_bracket = -94;	// ]
extern const char g_ctl_caret = -95;			// ^
extern const char g_ctl_left_brace = -124;		// {
extern const char g_ctl_pipe = -125;			// |
extern const char g_ctl_right_brace = -126;		// }




