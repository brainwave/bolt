vector3d planeN;
 float planeD;

float DistFromPlane( vector3d P)
{
    return dot(planeN,P) + planeD;
}

void GetSegmentPlaneIntersection( vector3d P1, vector3d P2, vector3dArray& outSegTips)
{
  float d1 = DistFromPlane(P1),
        d2 = DistFromPlane(P2);

  bool  bP1OnPlane = (abs(d1) < eps),
        bP2OnPlane = (abs(d2) < eps);

  if (bP1OnPlane)
     outSegTips.Add(P1);

  if (bP2OnPlane)
     outSegTips.Add(P2);

  if (bP1OnPlane && bP2OnPlane)
     return;

  if (d1*d2 > eps)  // points on the same side of plane
     return;

  float t = d1 / (d1 - d2); // 'time' of intersection point on the segment
  outSegTips.Add( P1 + t * (P2 - P1) );
}

void TrianglePlaneIntersection(vector3d triA, vector3d triB, vector3d triC,
                               vector3dArray& outSegTips)
{
   GetSegmentPlaneIntersection( triA, triB, outSegTips));
   GetSegmentPlaneIntersection( triB, triC, outSegTips));
   GetSegmentPlaneIntersection( triC, triA, outSegTips));

   RemoveDuplicates(outSegTips);  // not listed here - obvious functionality 
}
