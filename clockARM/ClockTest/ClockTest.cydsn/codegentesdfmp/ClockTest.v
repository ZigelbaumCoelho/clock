// ======================================================================
// ClockTest.v generated from TopDesign.cysch
// 07/11/2011 at 13:18
// ======================================================================

/* -- WARNING: The following section of defines are deprecated and will be removed in a future release -- */
`define CYDEV_CHIP_DIE_LEOPARD 1
`define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3
`define CYDEV_CHIP_REV_LEOPARD_ES3 3
`define CYDEV_CHIP_REV_LEOPARD_ES2 1
`define CYDEV_CHIP_REV_LEOPARD_ES1 0
`define CYDEV_CHIP_DIE_PANTHER 2
`define CYDEV_CHIP_REV_PANTHER_ES1 1
`define CYDEV_CHIP_REV_PANTHER_ES0 0
`define CYDEV_CHIP_DIE_EXPECT 1
`define CYDEV_CHIP_REV_EXPECT 3
`define CYDEV_CHIP_DIE_ACTUAL 1
/* -- WARNING: The previous section of defines are deprecated and will be removed in a future release -- */
`define CYDEV_CHIP_FAMILY_UNKNOWN 0
`define CYDEV_CHIP_MEMBER_UNKNOWN 0
`define CYDEV_CHIP_FAMILY_PSOC3 1
`define CYDEV_CHIP_MEMBER_3A 1
`define CYDEV_CHIP_REVISION_3A_PRODUCTION 3
`define CYDEV_CHIP_REVISION_3A_ES3 3
`define CYDEV_CHIP_REVISION_3A_ES2 1
`define CYDEV_CHIP_REVISION_3A_ES1 0
`define CYDEV_CHIP_FAMILY_PSOC5 2
`define CYDEV_CHIP_MEMBER_5A 2
`define CYDEV_CHIP_REVISION_5A_ES1 1
`define CYDEV_CHIP_REVISION_5A_ES0 0
`define CYDEV_CHIP_FAMILY_USED 1
`define CYDEV_CHIP_MEMBER_USED 1
`define CYDEV_CHIP_REVISION_USED 3
// Component: B_PWM_v2_0
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_PWM_v2_0"
`include "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_PWM_v2_0\B_PWM_v2_0.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_PWM_v2_0"
`include "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_PWM_v2_0\B_PWM_v2_0.v"
`endif

// Component: cy_virtualmux_v1_0
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`endif

// Component: OneTerminal
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal\OneTerminal.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal"
`include "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal\OneTerminal.v"
`endif

// Component: ZeroTerminal
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal\ZeroTerminal.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal"
`include "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal\ZeroTerminal.v"
`endif

// PWM_v2_0(CaptureMode=0, CompareStatusEdgeSense=true, CompareType1=1, CompareType1Software=0, CompareType2=1, CompareType2Software=0, CompareValue1=254, CompareValue2=63, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG8, CySetRegReplacementString=CY_SET_REG8, DeadBand=0, DeadBand2_4=0, DeadBandUsed=0, DeadTime=1, DitherOffset=0, EnableMode=0, FixedFunction=false, FixedFunctionUsed=0, InterruptOnCMP1=false, InterruptOnCMP2=false, InterruptOnKill=false, InterruptOnTC=false, IntOnCMP1=0, IntOnCMP2=0, IntOnKill=0, IntOnTC=0, KillMode=0, KillModeMinTime=0, MinimumKillTime=1, OneCompare=true, Period=255, PWMMode=0, PWMModeCenterAligned=0, RegDefReplacementString=reg8, RegSizeReplacementString=uint8, Resolution=8, RstStatusReplacementString=sSTSReg_rstSts, RunMode=0, TriggerMode=0, UseControl=true, UseInterrupt=true, UseStatus=true, VerilogSectionReplacementString=sP8, CY_COMPONENT_NAME=PWM_v2_0, CY_INSTANCE_SHORT_NAME=PWM, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=0, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=PWM, )
module PWM_v2_0_0 (pwm2, tc, clock, reset, pwm1, interrupt, capture, kill, enable, trigger, cmp_sel, pwm, ph1, ph2);
    output  pwm2;
    output  tc;
    input   clock;
    input   reset;
    output  pwm1;
    output  interrupt;
    input   capture;
    input   kill;
    input   enable;
    input   trigger;
    input   cmp_sel;
    output  pwm;
    output  ph1;
    output  ph2;

    parameter Resolution = 8;

    wire  Net_113;
    wire  Net_114;
    wire  Net_107;
    wire  Net_96;
    wire  Net_55;
    wire  Net_57;
    wire  Net_101;
    wire  Net_54;
    wire  Net_63;

    B_PWM_v2_0 PWMUDB (
        .reset(reset),
        .clock(clock),
        .tc(Net_101),
        .pwm1(pwm1),
        .pwm2(pwm2),
        .interrupt(Net_55),
        .kill(kill),
        .capture(capture),
        .enable(enable),
        .cmp_sel(cmp_sel),
        .trigger(trigger),
        .pwm(Net_96),
        .ph1(ph1),
        .ph2(ph2));
    defparam PWMUDB.CaptureMode = 0;
    defparam PWMUDB.CompareStatusEdgeSense = 1;
    defparam PWMUDB.CompareType1 = 1;
    defparam PWMUDB.CompareType2 = 1;
    defparam PWMUDB.DeadBand = 0;
    defparam PWMUDB.DitherOffset = 0;
    defparam PWMUDB.EnableMode = 0;
    defparam PWMUDB.KillMode = 0;
    defparam PWMUDB.PWMMode = 0;
    defparam PWMUDB.Resolution = 8;
    defparam PWMUDB.RunMode = 0;
    defparam PWMUDB.TriggerMode = 0;
    defparam PWMUDB.UseStatus = 1;

	// vmCompare (cy_virtualmux_v1_0)
	assign pwm = Net_96;

	// vmIRQ (cy_virtualmux_v1_0)
	assign interrupt = Net_55;

	// vmTC (cy_virtualmux_v1_0)
	assign tc = Net_101;

    OneTerminal OneTerminal_1 (
        .o(Net_113));

	// FFKillMux (cy_virtualmux_v1_0)
	assign Net_107 = Net_114;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_114));



endmodule

// Component: B_SPI_Master_v2_10
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_SPI_Master_v2_10"
`include "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_SPI_Master_v2_10\B_SPI_Master_v2_10.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_SPI_Master_v2_10"
`include "C:\Program Files\Cypress\PSoC Creator\1.0\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_SPI_Master_v2_10\B_SPI_Master_v2_10.v"
`endif

// SPI_Master_v2_10(BidirectMode=false, ClockInternal=false, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG8, CySetRegReplacementString=CY_SET_REG8, DesiredBitRate=1000000, InternalClockUsed=0, InternalInterruptEnabled=0, InternalRxInterruptEnabled=0, InternalTxInterruptEnabled=0, InterruptOnByteComplete=false, InterruptOnRXFull=false, InterruptOnRXNotEmpty=false, InterruptOnRXOverrun=false, InterruptOnSPIDone=false, InterruptOnSPIIdle=false, InterruptOnTXEmpty=false, InterruptOnTXNotFull=false, IntOnByteComp=0, IntOnRXFull=0, IntOnRXNotEmpty=0, IntOnRXOver=0, IntOnSPIDone=0, IntOnSPIIdle=0, IntOnTXEmpty=0, IntOnTXNotFull=0, Mode=1, ModeUseZero=1, NumberOfDataBits=8, RegDefReplacementString=reg8, RegSizeReplacementString=uint8, RxBufferSize=4, ShiftDir=0, TxBufferSize=4, UseInternalInterrupt=false, UseRxInternalInterrupt=false, UseTxInternalInterrupt=false, VerilogSectionReplacementString=sR8, CY_COMPONENT_NAME=SPI_Master_v2_10, CY_INSTANCE_SHORT_NAME=SPIM, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=10, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=SPIM, )
module SPI_Master_v2_10_1 (clock, reset, miso, sclk, mosi, ss, rx_interrupt, sdat, tx_interrupt);
    input   clock;
    input   reset;
    input   miso;
    output  sclk;
    output  mosi;
    output  ss;
    output  rx_interrupt;
    inout   sdat;
    output  tx_interrupt;

    parameter BidirectMode = 0;
    parameter NumberOfDataBits = 8;
    parameter ShiftDir = 0;

    wire  Net_257;
    wire  Net_273;
    wire  Net_274;
    wire  Net_244;
    wire  Net_239;
    wire  Net_253;
    wire  Net_161;
    wire  Net_10;

	// VirtualMux_1 (cy_virtualmux_v1_0)
	assign Net_10 = clock;

    B_SPI_Master_v2_10 BSPIM (
        .sclk(sclk),
        .ss(ss),
        .miso(Net_244),
        .clock(Net_10),
        .reset(Net_273),
        .rx_interpt(rx_interrupt),
        .tx_enable(Net_253),
        .mosi(mosi),
        .tx_interpt(tx_interrupt));
    defparam BSPIM.BidirectMode = 0;
    defparam BSPIM.ModeCPHA = 0;
    defparam BSPIM.ModePOL = 0;
    defparam BSPIM.NumberOfDataBits = 8;
    defparam BSPIM.ShiftDir = 0;

	// VirtualMux_2 (cy_virtualmux_v1_0)
	assign Net_244 = miso;

	// VirtualMux_3 (cy_virtualmux_v1_0)
	assign Net_273 = reset;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_274));



endmodule

// top
module top ;

    wire  Net_69;
    wire  Net_68;
    wire  Net_67;
    wire  Net_66;
    wire  Net_65;
    wire  Net_56;
    wire  Net_64;
    wire  Net_39;
    wire  Net_51;
    wire  Net_50;
    wire  Net_49;
    wire  Net_48;
    wire  Net_47;
    wire  Net_46;
    wire  Net_45;
    wire  Net_44;
    wire  Net_43;
    wire  Net_42;
    wire  Net_41;
    wire  Net_24;
    wire  Net_40;
    wire  Net_52;
    wire  Net_53;
    wire  Net_63;
    wire  Net_34;

	wire [0:0] tmpOE__OE_net;
	wire [0:0] tmpFB_0__OE_net;
	wire [0:0] tmpIO_0__OE_net;
	wire [0:0] tmpINTERRUPT_0__OE_net;
	electrical [0:0] tmpSIOVREF__OE_net;

	cy_psoc3_pins_v1_10
		#(.id("ed092b9b-d398-4703-be89-cebf998501f6"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		OE
		 (.oe(tmpOE__OE_net),
		  .y({Net_34}),
		  .fb({tmpFB_0__OE_net[0:0]}),
		  .io({tmpIO_0__OE_net[0:0]}),
		  .siovref(tmpSIOVREF__OE_net),
		  .interrupt({tmpINTERRUPT_0__OE_net[0:0]}));

	assign tmpOE__OE_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__LE_net;
	wire [0:0] tmpFB_0__LE_net;
	wire [0:0] tmpIO_0__LE_net;
	wire [0:0] tmpINTERRUPT_0__LE_net;
	electrical [0:0] tmpSIOVREF__LE_net;

	cy_psoc3_pins_v1_10
		#(.id("00ec895c-d1f8-4603-ac0d-ca2466bd8471"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		LE
		 (.oe(tmpOE__LE_net),
		  .y({Net_63}),
		  .fb({tmpFB_0__LE_net[0:0]}),
		  .io({tmpIO_0__LE_net[0:0]}),
		  .siovref(tmpSIOVREF__LE_net),
		  .interrupt({tmpINTERRUPT_0__LE_net[0:0]}));

	assign tmpOE__LE_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__SCLK_net;
	wire [0:0] tmpFB_0__SCLK_net;
	wire [0:0] tmpIO_0__SCLK_net;
	wire [0:0] tmpINTERRUPT_0__SCLK_net;
	electrical [0:0] tmpSIOVREF__SCLK_net;

	cy_psoc3_pins_v1_10
		#(.id("64e61631-9930-471b-97b6-b1c676b6e132"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		SCLK
		 (.oe(tmpOE__SCLK_net),
		  .y({Net_53}),
		  .fb({tmpFB_0__SCLK_net[0:0]}),
		  .io({tmpIO_0__SCLK_net[0:0]}),
		  .siovref(tmpSIOVREF__SCLK_net),
		  .interrupt({tmpINTERRUPT_0__SCLK_net[0:0]}));

	assign tmpOE__SCLK_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__DATA_net;
	wire [0:0] tmpFB_0__DATA_net;
	wire [0:0] tmpIO_0__DATA_net;
	wire [0:0] tmpINTERRUPT_0__DATA_net;
	electrical [0:0] tmpSIOVREF__DATA_net;

	cy_psoc3_pins_v1_10
		#(.id("3f3246ce-84f8-4cce-b011-9d40a0443957"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		DATA
		 (.oe(tmpOE__DATA_net),
		  .y({Net_52}),
		  .fb({tmpFB_0__DATA_net[0:0]}),
		  .io({tmpIO_0__DATA_net[0:0]}),
		  .siovref(tmpSIOVREF__DATA_net),
		  .interrupt({tmpINTERRUPT_0__DATA_net[0:0]}));

	assign tmpOE__DATA_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    PWM_v2_0_0 PWM (
        .reset(1'b0),
        .clock(Net_24),
        .tc(Net_41),
        .pwm1(Net_42),
        .pwm2(Net_43),
        .interrupt(Net_44),
        .capture(1'b0),
        .kill(1'b1),
        .enable(1'b1),
        .trigger(1'b0),
        .cmp_sel(1'b0),
        .pwm(Net_34),
        .ph1(Net_50),
        .ph2(Net_51));
    defparam PWM.Resolution = 8;


	cy_clock_v1_0
		#(.id("04a5ce28-63ac-492b-ab8c-c309a86b9fe2"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		Clock_1
		 (.clock_out(Net_24));


    SPI_Master_v2_10_1 SPIM (
        .mosi(Net_52),
        .sclk(Net_53),
        .ss(Net_63),
        .miso(Net_64),
        .clock(Net_56),
        .reset(1'b0),
        .rx_interrupt(Net_66),
        .sdat(Net_67),
        .tx_interrupt(Net_68));
    defparam SPIM.BidirectMode = 0;
    defparam SPIM.NumberOfDataBits = 8;
    defparam SPIM.ShiftDir = 0;


	cy_clock_v1_0
		#(.id("5e7dc2e8-5765-467f-9d88-654b20ba1333"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("62500000"),
		  .is_direct(0),
		  .is_digital(1))
		Clock_2
		 (.clock_out(Net_56));


    ZeroTerminal ZeroTerminal_1 (
        .z(Net_64));



endmodule

