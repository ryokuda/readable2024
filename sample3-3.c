int isCrossing =
      (((xa>x0) && (xa<x1) && (ya>y0) && (ya<y1)) &&
       ((xb<x0) || (xb>x1) || (yb<y0) || (yb>y1))) ||
      (((xa<x0) || (xa>x1) || (ya<y0) || (ya>y1)) &&
       ((xb>x0) && (xb<x1) && (yb>y0) && (yb<y1)));
