#include "bsp_elog.h"


void bsp_elog_init(void)
{
	elog_init();
//	elog_set_text_color_enable(true);
	
	elog_set_fmt(ELOG_LVL_ASSERT	,	ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_WARN		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_INFO		,	ELOG_FMT_LVL|ELOG_FMT_TAG);
	elog_set_fmt(ELOG_LVL_DEBUG		,	ELOG_FMT_ALL & ~(	ELOG_FMT_TIME	|
															ELOG_FMT_P_INFO	|
															ELOG_FMT_T_INFO	));
	elog_set_fmt(ELOG_LVL_VERBOSE	,	ELOG_FMT_ALL);
	
	elog_start();
}

void test_elog(void)
{
	log_a("this assert");
	log_e("this is error");
	log_w("this is warning");
	log_i("this is info");
	log_d("this is debug");
	log_v("this is verbose");
}

