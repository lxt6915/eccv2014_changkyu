#include "mvt.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

MVT_Param g_param;

MVT_Param ParseArguments(int argc, char **argv)
{
	po::options_description desc("\nAvailable Options for 'Multiview Tracker'\n");

	desc.add_options()
	(MVT_PARAM__HELP,                                             "produce help message")
	(MVT_PARAM__IMG_PATH,        po::value<std::string>(),       "The path for target images")
	(MVT_PARAM__IMG_LIST,        po::value<std::string>(),       "The image list")
	(MVT_PARAM__IMG_NAMEFORMAT,  po::value<std::string>(),       "The image name format")
	(MVT_PARAM__DPMCONF_PATH,    po::value<std::string>(),       "The path for dpm confidence files")
	(MVT_PARAM__DPMCONF_NAMEFORMAT, po::value<std::string>(),    "The conf name format")
	(MVT_PARAM__IMG_START_FRAME, po::value<unsigned int>(),    "The start frame")
	(MVT_PARAM__IMG_END_FRAME,   po::value<unsigned int>(),    "The end frame")
	(MVT_PARAM__RESULT_FILE,     po::value<std::string>(),       "The path for tracking result")
	(MVT_PARAM__LOG_FILE,        po::value<std::string>(),       "The path for tracking log")
	(MVT_PARAM__DPM_ONOFF,       po::value<bool>(),             "Using DPM detection response")
	(MVT_PARAM__ALM_ONOFF,       po::value<bool>(),             "Using ALM detection response")
	(MVT_PARAM__MIL_ONOFF,       po::value<bool>(),             "Using MIL online learning model response")
	(MVT_PARAM__PAIRWISE_ONOFF,  po::value<bool>(),             "Using Pairwise penalty function")
	(MVT_PARAM__PRIOR_ONOFF,     po::value<bool>(),             "Using Prior penalty function")
	(MVT_PARAM__USE_MIL_ROOT,   po::value<bool>(),               "Using MIL root part")
	(MVT_PARAM__VIS,             po::value<bool>(),             "Visualize the tracking results")
	(MVT_PARAM__INIT_ONOFF,      po::value<bool>(),           "Initial Status: center point x")
	(MVT_PARAM__INIT_STATE_X,    po::value<double>(),           "Initial Status: center point x")
	(MVT_PARAM__INIT_STATE_Y,    po::value<double>(),           "Initial Status: center point y")
	(MVT_PARAM__INIT_STATE_A,    po::value<double>(),           "Initial Status: viewpoint azimuth")
	(MVT_PARAM__INIT_STATE_E,    po::value<double>(),           "Initial Status: viewpoint elevation")
	(MVT_PARAM__INIT_STATE_D,    po::value<double>(),           "Initial Status: viewpoint distance")
	(MVT_PARAM__OBJECT_CATEGORY    ,po::value<std::string>(),    "object category for the target object")
	(MVT_PARAM__3DOBJECT_PATH      ,po::value<std::string>(),    "Path for 3d-Object model")
	(MVT_PARAM__NUM_OF_VIEWPOINT   ,po::value<unsigned int>(),  "Sampling Option: the number of viewpoint")
	(MVT_PARAM__NUM_OF_CENTER      ,po::value<unsigned int>(), "Sampling Option: the number of hypothesis per a viewpoint")
	(MVT_PARAM__NUM_OF_PARTCENTER      ,po::value<unsigned int>(), "Sampling Option: the number of hypothesis per a viewpoint")
	(MVT_PARAM__STD_AZIMUTH        ,po::value<double>(),        "Sampling Option: standard derivation for azimuth")
	(MVT_PARAM__STD_ELEVATION      ,po::value<double>(),        "Sampling Option: standard derivation for elevation")
	(MVT_PARAM__STD_DISTANCE       ,po::value<double>(),        "Sampling Option: standard derivation for distance")
	(MVT_PARAM__STD_PRIOR_AZIMUTH        ,po::value<double>(),        "PRIOR Option: standard derivation for azimuth")
	(MVT_PARAM__STD_PRIOR_ELEVATION      ,po::value<double>(),        "PRIOR Option: standard derivation for elevation")
	(MVT_PARAM__STD_PRIOR_DISTANCE       ,po::value<double>(),        "PRIOR Option: standard derivation for distance")
	(MVT_PARAM__SEARCH_WINDOW_SIZE ,po::value<unsigned int>(), "Searching Window Size")
	(MVT_PARAM__THRESH_DPM         ,po::value<double>(), "Threshold for DPM")
	(MVT_PARAM__THRESH2_DPM        ,po::value<double>(), "Threshold2 for DPM")
	(MVT_PARAM__THRESH_ALM         ,po::value<double>(), "Threshold for ALM")
	(MVT_PARAM__THRESH_MIL         ,po::value<double>(), "Threshold for MIL")
	(MVT_PARAM__WEIGHT_MIL_ROOT    ,po::value<double>(), "Root Weight for MIL")
	(MVT_PARAM__HEIGHT_MIL_ROOT     ,po::value<unsigned int>(), "Root Fixed Height for MIL")

	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	MVT_Param param;
	if (vm.count(MVT_PARAM__HELP))
	{
		LOG( desc << std::endl );
		exit(1);
	}
	//////////////////////////////////////////////////////////////////
	// input data
	//////////////////////////////////////////////////////////////////
	if (vm.count(MVT_PARAM__IMG_PATH))
	{
		g_param.path_imgs = vm[MVT_PARAM__IMG_PATH].as<std::string>();
		if( g_param.path_imgs.at(g_param.path_imgs.size()-1) != '/' )
		{
			g_param.path_imgs.append("/");
		}
	}
	else
		exit(0);

	if (vm.count(MVT_PARAM__IMG_LIST))
		g_param.filepath_imglist = vm[MVT_PARAM__IMG_LIST].as<std::string>();
	else if (vm.count(MVT_PARAM__IMG_NAMEFORMAT))
		g_param.namefmt_imgs = vm[MVT_PARAM__IMG_NAMEFORMAT].as<std::string>();
	else
		exit(0);

	if (vm.count(MVT_PARAM__IMG_START_FRAME))
		g_param.idx_img_start = vm[MVT_PARAM__IMG_START_FRAME].as<unsigned int>();
	else
		g_param.idx_img_start = 1;

	if (vm.count(MVT_PARAM__IMG_END_FRAME))
			g_param.idx_img_end = vm[MVT_PARAM__IMG_END_FRAME].as<unsigned int>();
	else
		g_param.idx_img_end = 0;

	if (vm.count(MVT_PARAM__RESULT_FILE))
		g_param.filepath_result = vm[MVT_PARAM__RESULT_FILE].as<std::string>();
	else
		exit(0);

	if (vm.count(MVT_PARAM__LOG_FILE))
		g_param.filepath_log = vm[MVT_PARAM__LOG_FILE].as<std::string>();
	else
		g_param.filepath_log = "./log.txt";

	if (vm.count(MVT_PARAM__DPM_ONOFF))
	{
		g_param.use_dpm = vm[MVT_PARAM__DPM_ONOFF].as<bool>();
		if( g_param.use_dpm )
		{
			if (vm.count(MVT_PARAM__DPMCONF_PATH) && vm.count(MVT_PARAM__DPMCONF_NAMEFORMAT))
			{
				g_param.path_dpmconfs = vm[MVT_PARAM__DPMCONF_PATH].as<std::string>();
				g_param.namefmt_dpmconfs = vm[MVT_PARAM__DPMCONF_NAMEFORMAT].as<std::string>();
			}
			else
			{
				exit(0);
			}
		}
	}
	else
		g_param.use_dpm = true;

	if (vm.count(MVT_PARAM__ALM_ONOFF))
		g_param.use_alm = vm[MVT_PARAM__ALM_ONOFF].as<bool>();
	else
		g_param.use_alm = true;

	if (vm.count(MVT_PARAM__MIL_ONOFF))
		g_param.use_mil = vm[MVT_PARAM__MIL_ONOFF].as<bool>();
	else
		g_param.use_mil = true;

	if (vm.count(MVT_PARAM__PAIRWISE_ONOFF))
		g_param.use_pairwise = vm[MVT_PARAM__PAIRWISE_ONOFF].as<bool>();
	else
		g_param.use_pairwise = true;

	if (vm.count(MVT_PARAM__PRIOR_ONOFF))
		g_param.use_prior = vm[MVT_PARAM__PRIOR_ONOFF].as<bool>();
	else
		g_param.use_prior = true;

	if (vm.count(MVT_PARAM__USE_MIL_ROOT))
		g_param.use_mil_root = vm[MVT_PARAM__USE_MIL_ROOT].as<bool>();
	else
		g_param.use_mil_root = false;

	if (vm.count(MVT_PARAM__VIS))
		g_param.is_vis = vm[MVT_PARAM__VIS].as<bool>();
	else
		g_param.is_vis = false;

	if (vm.count(MVT_PARAM__INIT_ONOFF))
		g_param.use_init = vm[MVT_PARAM__INIT_ONOFF].as<bool>();
	else
		g_param.use_init = true;

	if( g_param.use_init )
	{
		if (vm.count(MVT_PARAM__INIT_STATE_X))
			g_param.init_state_x = vm[MVT_PARAM__INIT_STATE_X].as<double>();
		else
			exit(0);

		if (vm.count(MVT_PARAM__INIT_STATE_Y))
			g_param.init_state_y = vm[MVT_PARAM__INIT_STATE_Y].as<double>();
		else
			exit(0);

		if (vm.count(MVT_PARAM__INIT_STATE_A))
			g_param.init_state_a = vm[MVT_PARAM__INIT_STATE_A].as<double>();
		else
			exit(0);

		if (vm.count(MVT_PARAM__INIT_STATE_E))
			g_param.init_state_e = vm[MVT_PARAM__INIT_STATE_E].as<double>();
		else
			exit(0);

		if (vm.count(MVT_PARAM__INIT_STATE_D))
			g_param.init_state_d = vm[MVT_PARAM__INIT_STATE_D].as<double>();
		else
			exit(0);
	}

	if (vm.count(MVT_PARAM__OBJECT_CATEGORY))
	{
		std::string str_category = vm[MVT_PARAM__OBJECT_CATEGORY].as<std::string>();
		if(str_category.compare("car") == 0)
			g_param.object_category = OBJECT_CATEGORY_CAR;
		else
			exit(0);
	}
	else
		exit(0);

	if (vm.count(MVT_PARAM__3DOBJECT_PATH))
		g_param.filepath_3dobject_model = vm[MVT_PARAM__3DOBJECT_PATH].as<std::string>();
	else
		exit(0);

	if (vm.count(MVT_PARAM__NUM_OF_VIEWPOINT))
		g_param.num_of_viewpoint_sample = vm[MVT_PARAM__NUM_OF_VIEWPOINT].as<unsigned int>();
	else
		g_param.num_of_viewpoint_sample = 30;

	if (vm.count(MVT_PARAM__STD_AZIMUTH))
		g_param.std_azimuth = vm[MVT_PARAM__STD_AZIMUTH].as<double>();
	else
		g_param.std_azimuth = 10;

	if (vm.count(MVT_PARAM__STD_ELEVATION))
		g_param.std_elevation = vm[MVT_PARAM__STD_ELEVATION].as<double>();
	else
		g_param.std_elevation = 1;

	if (vm.count(MVT_PARAM__STD_DISTANCE))
		g_param.std_distance = vm[MVT_PARAM__STD_DISTANCE].as<double>();
	else
		g_param.std_distance = 1;

	if (vm.count(MVT_PARAM__STD_PRIOR_AZIMUTH))
		g_param.std_prior_azimuth = vm[MVT_PARAM__STD_PRIOR_AZIMUTH].as<double>();
	else
		g_param.std_prior_azimuth = 45;

	if (vm.count(MVT_PARAM__STD_PRIOR_ELEVATION))
		g_param.std_prior_elevation = vm[MVT_PARAM__STD_PRIOR_ELEVATION].as<double>();
	else
		g_param.std_prior_elevation = 5;

	if (vm.count(MVT_PARAM__STD_PRIOR_DISTANCE))
		g_param.std_prior_distance = vm[MVT_PARAM__STD_PRIOR_DISTANCE].as<double>();
	else
		g_param.std_prior_distance = 3;


	if (vm.count(MVT_PARAM__NUM_OF_CENTER))
		g_param.num_of_center_sample = vm[MVT_PARAM__NUM_OF_CENTER].as<unsigned int>();
	else
		g_param.num_of_center_sample = 30;

	if (vm.count(MVT_PARAM__NUM_OF_PARTCENTER))
		g_param.num_of_partcenter_sample = vm[MVT_PARAM__NUM_OF_PARTCENTER].as<unsigned int>();
	else
		g_param.num_of_partcenter_sample = 30;

	if (vm.count(MVT_PARAM__SEARCH_WINDOW_SIZE))
		g_param.srchwinsz = vm[MVT_PARAM__SEARCH_WINDOW_SIZE].as<unsigned int>();
	else
		g_param.srchwinsz = 500;

	if (vm.count(MVT_PARAM__THRESH_DPM))
		g_param.thresh_dpm = vm[MVT_PARAM__THRESH_DPM].as<double>();
	else
		g_param.thresh_dpm = 0.6;

	if (vm.count(MVT_PARAM__THRESH2_DPM))
		g_param.thresh2_dpm = vm[MVT_PARAM__THRESH2_DPM].as<double>();
	else
		g_param.thresh2_dpm = 0.3;

	if (vm.count(MVT_PARAM__THRESH_ALM))
		g_param.thresh_alm = vm[MVT_PARAM__THRESH_ALM].as<double>();
	else
		g_param.thresh_alm = -10000;

	if (vm.count(MVT_PARAM__THRESH_MIL))
		g_param.thresh_mil = vm[MVT_PARAM__THRESH_MIL].as<double>();
	else
		g_param.thresh_mil = -1000;

	if (vm.count(MVT_PARAM__WEIGHT_MIL_ROOT))
		g_param.weight_mil_root = vm[MVT_PARAM__WEIGHT_MIL_ROOT].as<double>();
	else
		g_param.weight_mil_root = 6;

	if (vm.count(MVT_PARAM__HEIGHT_MIL_ROOT))
		g_param.height_mil_root = vm[MVT_PARAM__HEIGHT_MIL_ROOT].as<unsigned int>();
	else
		g_param.height_mil_root = 100;


	/* MIL Online Model */
	g_param.param_mil.algorithm_       = cv::ObjectTrackerParams::CV_ONLINEMIL;
	g_param.param_mil.num_classifiers_ = 50;
	g_param.param_mil.num_features_    = 250;

	return g_param;
}

void PrintArguments()
{
	LOG( "---< Parameter Settings >-------------------------------" << std::endl );
	LOG( MVT_PARAM__IMG_PATH           << ": " << g_param.path_imgs << std::endl );
	LOG( MVT_PARAM__IMG_LIST           << ": " << g_param.filepath_imglist << std::endl );
	LOG( MVT_PARAM__IMG_NAMEFORMAT     << ": " << g_param.namefmt_imgs << std::endl );
	LOG( MVT_PARAM__IMG_START_FRAME    << ": " << g_param.idx_img_start << std::endl );
	LOG( MVT_PARAM__IMG_END_FRAME      << ": " << g_param.idx_img_end << std::endl );
	LOG( MVT_PARAM__DPMCONF_PATH       << ": " << g_param.path_dpmconfs << std::endl );
	LOG( MVT_PARAM__DPMCONF_NAMEFORMAT << ": " << g_param.namefmt_dpmconfs << std::endl );
	LOG( MVT_PARAM__RESULT_FILE        << ": " << g_param.filepath_result << std::endl );
	LOG( MVT_PARAM__DPM_ONOFF          << ": " << g_param.use_dpm << std::endl );
	LOG( MVT_PARAM__ALM_ONOFF          << ": " << g_param.use_alm << std::endl );
	LOG( MVT_PARAM__MIL_ONOFF          << ": " << g_param.use_mil << std::endl );
	LOG( MVT_PARAM__PAIRWISE_ONOFF     << ": " << g_param.use_pairwise << std::endl );
	LOG( MVT_PARAM__PRIOR_ONOFF        << ": " << g_param.use_prior    << std::endl );
	LOG( MVT_PARAM__USE_MIL_ROOT       << ": " << g_param.use_mil_root << std::endl );
	LOG( MVT_PARAM__VIS                << ": " << g_param.is_vis << std::endl );

	LOG( MVT_PARAM__INIT_ONOFF	       << ": " << g_param.use_init     << std::endl );
	LOG( MVT_PARAM__INIT_STATE_X	   << ": " << g_param.init_state_x << std::endl );
	LOG( MVT_PARAM__INIT_STATE_Y	   << ": " << g_param.init_state_y << std::endl );
	LOG( MVT_PARAM__INIT_STATE_A	   << ": " << g_param.init_state_a << std::endl );
	LOG( MVT_PARAM__INIT_STATE_E	   << ": " << g_param.init_state_e << std::endl );
	LOG( MVT_PARAM__INIT_STATE_D	   << ": " << g_param.init_state_d << std::endl );

	LOG( MVT_PARAM__OBJECT_CATEGORY    << ": " << g_param.object_category << std::endl );
	LOG( MVT_PARAM__3DOBJECT_PATH      << ": " << g_param.filepath_3dobject_model << std::endl );
	LOG( MVT_PARAM__NUM_OF_VIEWPOINT   << ": " << g_param.num_of_viewpoint_sample << std::endl );
	LOG( MVT_PARAM__NUM_OF_CENTER      << ": " << g_param.num_of_center_sample << std::endl );
	LOG( MVT_PARAM__NUM_OF_PARTCENTER  << ": " << g_param.num_of_partcenter_sample << std::endl );
	LOG( MVT_PARAM__STD_AZIMUTH        << ": " << g_param.std_azimuth << std::endl );
	LOG( MVT_PARAM__STD_ELEVATION      << ": " << g_param.std_elevation << std::endl );
	LOG( MVT_PARAM__STD_DISTANCE       << ": " << g_param.std_distance << std::endl );

	LOG( MVT_PARAM__STD_PRIOR_AZIMUTH        << ": " << g_param.std_prior_azimuth << std::endl );
	LOG( MVT_PARAM__STD_PRIOR_ELEVATION      << ": " << g_param.std_prior_elevation << std::endl );
	LOG( MVT_PARAM__STD_PRIOR_DISTANCE       << ": " << g_param.std_prior_distance << std::endl );

	LOG( MVT_PARAM__THRESH_DPM        << ": " << g_param.thresh_dpm << std::endl );
	LOG( MVT_PARAM__THRESH2_DPM        << ": " << g_param.thresh2_dpm << std::endl );
	LOG( MVT_PARAM__THRESH_ALM        << ": " << g_param.thresh_alm << std::endl );
	LOG( MVT_PARAM__THRESH_MIL        << ": " << g_param.thresh_mil << std::endl );

	LOG( MVT_PARAM__WEIGHT_MIL_ROOT        << ": " << g_param.weight_mil_root << std::endl );
	LOG( MVT_PARAM__HEIGHT_MIL_ROOT        << ": " << g_param.height_mil_root << std::endl );

	LOG( MVT_PARAM__SEARCH_WINDOW_SIZE << ": " << g_param.srchwinsz << std::endl );

	LOG( "--------------------------------------------------------" << std::endl );
	LOG( std::endl );
}
