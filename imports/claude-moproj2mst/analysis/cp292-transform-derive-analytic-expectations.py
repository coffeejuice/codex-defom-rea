# Independent analytic expectations for selected CP292 fixtures. Decimal sqrt
# plus vector determinants replace the native slope-based intersection path.
# This is not a generic copy of the transform or a runtime reference oracle.
from decimal import Decimal as D,localcontext
import json
with localcontext() as ctx:
 ctx.prec=80
 z=D(0);one=D(1);two=D(2);four=D(4);five=D(5)
 def add(a,b):return (a[0]+b[0],a[1]+b[1])
 def sub(a,b):return (a[0]-b[0],a[1]-b[1])
 def times(k,a):return (k*a[0],k*a[1])
 def dot(a,b):return a[0]*b[0]+a[1]*b[1]
 def norm(a):return times(one/dot(a,a).sqrt(),a)
 def det(a,b):return a[0]*b[1]-a[1]*b[0]
 def meet(a,u,b,v):return add(a,times(det(sub(b,a),v)/det(u,v),u))
 r5=five.sqrt();h=one/two.sqrt()
 n0=(h,h)
 n1=norm((-one/r5,one-two/r5))
 n2=norm((one-one/r5,-two/r5))
 k0=add(add(n0,times(D('2.5'),norm(add(n0,n2)))),times(D('.625'),norm(add(n0,n1))))
 k1=add(add(n1,times(D('.625'),norm(add(n1,n0)))),times(D('.5'),norm(add(n1,n2))))
 # At C the directional x<0 limiter chooses c=-u+.2*v, independently
 # established by signs; normalization/scales do not affect its ray.
 k2=(two/r5,-one/r5-D('.2'))
 a=(z,z);b=(four,z);c=(z,two)
 q01=meet(a,k0,b,k1);q12=meet(b,k1,c,k2);q20=meet(c,k2,a,k0)
 initial=[times(one/k0[1],k0),add(b,times(five,norm(k1))),add(c,times(five,norm(k2)))]
 def vec(q):return [str(x) for x in q]
 # Sequential fixture: n1 angle67.5; neighboring sum angles56.25/101.25.
 # Half angles give sin/cos using only nested roots, no transform code.
 def coshalf(c):return ((one+c)/two).sqrt()
 def sinhalf(c):return ((one-c)/two).sqrt()
 cos45=h;cos22=coshalf(cos45);sin22=sinhalf(cos45)
 cos112=-sin22 # cos(112.5)
 cos56=coshalf(cos112);sin56=sinhalf(cos112)
 # 101.25 = 90+11.25, with 11.25 the half angle of22.5.
 cos11=coshalf(cos22);sin11=sinhalf(cos22)
 nn1=(sin22,cos22)
 ray=add(add(nn1,times(D('11.25'),(cos56,sin56))),times(D(90),(-sin11,cos11)))
 t=four*(ray[0]+ray[1])/(ray[1]-ray[0])
 result={'qualification':'analytic expectations, no original runtime parity',
 'wide_triangle':{'old_normals':[vec(n) for n in (n0,n1,n2)],'rays':[vec(k) for k in (k0,k1,k2)],'initial':[vec(x) for x in initial],
  'Q01':vec(q01),'Q12':vec(q12),'Q20':vec(q20),'distance_first':str(dot(q01,q01).sqrt()),'distance_last':str(dot(q20,q20).sqrt()),
  'expected':[vec(q) for q in (q01,q12,q20,q01)]},
 'tall_triangle':{'expected':[vec((q[1],q[0])) for q in (q01,q20,q12,q01)],'branch':'lastToFirst'},
 'sequential':{'quad_ray_coordinate':str(t),'near_crossing':['3','3'],'far_crossing':['3.12','3.12']}}
 print(json.dumps(result,indent=2))
