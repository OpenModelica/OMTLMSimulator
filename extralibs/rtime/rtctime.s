	.file	"rtctime.s"
	
	.align	2
#	.globl	rtctime[ds]
#	.csect	rtctime[ds]
	.long	.rtctime, TOC[tc0], 0
	.globl	.rtctime
	.csect	.rtctime[pr]
.rtctime:
	mfspr	r4,4
	mfspr	r5,5
	mfspr	r6,4
	cmp	0x7,0x0,r4,r6
	bc	0x4,0x1e,.rtctime
	stw	r4,0(r3)
	stw	r5,4(r3)
	bclr	0x14,0x0
