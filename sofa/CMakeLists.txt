set(SOFASRC a2af.c		c2tcio.c	fw2m.c		pmat76.c	s2c.c
a2tf.c		c2teqx.c	fw2xy.c		pmp.c		s2p.c
ab.c		c2tpe.c		gc2gd.c		pmpx.c		s2pv.c
af2a.c		c2txy.c		gc2gde.c	pmsafe.c	s2xpv.c
anp.c		cal2jd.c	gd2gc.c		pn.c		sepp.c
anpm.c		cp.c		gd2gce.c	pn00.c		seps.c
apcg.c		cpv.c		gmst00.c	pn00a.c		sofa.h
apcg13.c	cr.c		gmst06.c	pn00b.c
apci.c		d2dtf.c		gmst82.c	pn06.c		sofam.h
apci13.c	d2tf.c		gst00a.c	pn06a.c		sp00.c
apco.c		dat.c		gst00b.c	pnm00a.c	starpm.c
apco13.c	dtdb.c		gst06.c		pnm00b.c	starpv.c
apcs.c		dtf2d.c		gst06a.c	pnm06a.c	sxp.c
apcs13.c	ee00.c		gst94.c		pnm80.c		sxpv.c
aper.c		ee00a.c		h2fk5.c		pom00.c		t_sofa_c.c
aper13.c	ee00b.c		hfk5z.c		ppp.c		taitt.c
apio.c		ee06a.c		ir.c		ppsp.c		taiut1.c
apio13.c	eect00.c	jd2cal.c	pr00.c		taiutc.c
atci13.c	eform.c		jdcalf.c	prec76.c	tcbtdb.c
atciq.c		eo06a.c		ld.c		pv2p.c		tcgtt.c
atciqn.c	eors.c		ldn.c		pv2s.c		tdbtcb.c
atciqz.c	epb.c		ldsun.c		pvdpv.c		tdbtt.c
atco13.c	epb2jd.c	num00a.c	pvm.c		tf2a.c
atic13.c	epj.c		num00b.c	pvmpv.c		tf2d.c
aticq.c		epj2jd.c	num06a.c	pvppv.c		tr.c
aticqn.c	epv00.c		numat.c		pvstar.c	trxp.c
atio13.c	eqeq94.c	nut00a.c	pvtob.c		trxpv.c
atioq.c		era00.c		nut00b.c	pvu.c		tttai.c
atoc13.c	fad03.c		nut06a.c	pvup.c		tttcg.c
atoi13.c	fae03.c		nut80.c		pvxpv.c		tttdb.c
atoiq.c		faf03.c		nutm80.c	pxp.c		ttut1.c
bi00.c		faju03.c	obl06.c		refco.c		ut1tai.c
bp00.c		fal03.c		obl80.c		rm2v.c		ut1tt.c
bp06.c		falp03.c	p06e.c		rv2m.c		ut1utc.c
bpn2xy.c	fama03.c	p2pv.c		rx.c		utctai.c
c2i00a.c	fame03.c	p2s.c		rxp.c		utcut1.c
c2i00b.c	fane03.c	pap.c		rxpv.c		xy06.c
c2i06a.c	faom03.c	pas.c		rxr.c		xys00a.c
c2ibpn.c	fapa03.c	pb06.c		ry.c		xys00b.c
c2ixy.c		fasa03.c	pdp.c		rz.c		xys06a.c
c2ixys.c	faur03.c	pfw06.c		s00.c		zp.c
c2s.c		fave03.c	plan94.c	s00a.c		zpv.c
c2t00a.c	fk52h.c		pm.c		s00b.c		zr.c
c2t00b.c	fk5hip.c	pmat00.c	s06.c
c2t06a.c	fk5hz.c		pmat06.c	s06a.c)

add_library(sofa SHARED $(SOFASRC})
