#include <field_pipelines.h>
#include <math.h> /* For sqrt */

double
compute_rms_div_b_err( field_t * ALIGNED f,
                       const grid_t * g ) {
  compute_rms_div_b_err_pipeline_args_t args[1];
  pipeline_request_t request[1];
  int p;
  
  double err = 0, local[2], global[2];

  if( f==NULL ) { ERROR(("Bad field")); return -1; }
  if( g==NULL ) { ERROR(("Bad grid"));  return -1; }

  args->f = f;
  args->g = g;
  dispatch_pipelines( compute_rms_div_b_err_pipeline, args, 0, request );
  wait_for_pipelines( request );

  err = 0;
  for( p=0; p<n_pipeline; p++ ) err += args->err[p];
  local[0] = err*g->dx*g->dy*g->dz;
  local[1] = g->nx*g->ny*g->nz*g->dx*g->dy*g->dz;
  mp_allsum_d( local, global, 2, g->mp );
  return g->eps0*sqrt(global[0]/global[1]);
}

