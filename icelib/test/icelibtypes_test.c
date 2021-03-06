
#include <check.h>

#include "icelibtypes.h"

START_TEST( candidate_toString )
{
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(ICE_CAND_TYPE_NONE), "NONE") );
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(ICE_CAND_TYPE_HOST), "HOST") );
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(ICE_CAND_TYPE_SRFLX), "SRFLX") );    
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(ICE_CAND_TYPE_RELAY), "RELAY") );
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(ICE_CAND_TYPE_PRFLX), "PRFLX") );
    fail_unless( 0 == strcmp(ICELIBTYPES_ICE_CANDIDATE_TYPE_toString(24), "UNKNOWN") );

}
END_TEST


START_TEST( iceMedia_empty )
{
    ICE_MEDIA iceMedia;
    ICELIBTYPES_ICE_MEDIA_reset(&iceMedia);


    fail_unless( ICELIBTYPES_ICE_MEDIA_isEmpty(&iceMedia) );

    iceMedia.numberOfICEMediaLines = 2;

    fail_if( ICELIBTYPES_ICE_MEDIA_isEmpty(&iceMedia) );

}
END_TEST

START_TEST( mediastream_empty ){
    ICE_MEDIA_STREAM iceMediaStream;

    ICELIBTYPES_ICE_MEDIA_STREAM_reset(&iceMediaStream);
    fail_unless( ICELIBTYPES_ICE_MEDIA_STREAM_isEmpty(&iceMediaStream) );
    
    iceMediaStream.numberOfCandidates = 3;

    fail_if( ICELIBTYPES_ICE_MEDIA_STREAM_isEmpty(&iceMediaStream) );


}
END_TEST

Suite * icelibtypes_suite (void)
{
  Suite *s = suite_create ("ICElibtypes");

  {/* Core test case */
      TCase *tc_core = tcase_create ("Core");
      tcase_add_test (tc_core, candidate_toString);
      tcase_add_test (tc_core, iceMedia_empty );
      tcase_add_test (tc_core, mediastream_empty );
      suite_add_tcase (s, tc_core);
  }

  return s;
}
