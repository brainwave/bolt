
// Assume the plane is given as the equation dot(N,X) + d = 0, where N is a (not
// neccessarily normalized) plane normal, and d is a scalar. Any way the plane is given -
// DistFromPlane should just let the input vector into the plane equation.

vector3d planeN;
float planeD;

float DistFromPlane( vector3d P)
{
// if N is not normalized this is *not* really the distance, 
// but the computations work just the same.
    return dot(planeN,P) + planeD;
}

bool GetSegmentPlaneIntersection( vector3d P1, vector3d P2, vector3d& outP)
{
  float d1 = DistFromPlane(P1),
        d2 = DistFromPlane(P2);

  if (d1*d2 > 0)  // points on the same side of plane
     return false;

  float t = d1 / (d1 - d2); // 'time' of intersection point on the segment
  outP = P1 + t * (P2 - P1);

  return true;
}

void TrianglePlaneIntersection(vector3d triA, vector3d triB, vector3d triC,
                               vector3dArray& outSegTips)
{
   vector3d IntersectionPoint;
   if( GetSegmentPlaneIntersection( triA, triB, IntersectionPoint))
     outSegTips.Add(IntersectionPoint);

   if( GetSegmentPlaneIntersection( triB, triC, IntersectionPoint))
     outSegTips.Add(IntersectionPoint);

   if( GetSegmentPlaneIntersection( triC, triA, IntersectionPoint))
     outSegTips.Add(IntersectionPoint);
}
