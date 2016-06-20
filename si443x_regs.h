#ifndef si443x_regs_included
#define si443x_regs_included

/*---------------------------------------------*/
#define si_DT 0x00
#define si_EZRadioPRO 0x08
/*---------------------------------------------*/
#define si_VC 0x01
#define si_443xB1 0x06
#define si_10xxA  0x06
#define si_10xxB  0x07
/*---------------------------------------------*/
#define si_status 0x02
#define si_ffovfl     (1<<7)
#define si_ffunfl     (1<<6)
#define si_rxffem     (1<<5)
#define si_headerr    (1<<4)
#define si_freqerr    (1<<3)
#define si_cps_mask   0x03
#define si_cps_idle   0x00
#define si_cps_rx     0x01
#define si_cps_tx     0x02
/*---------------------------------------------*/
#define si_interrupt1 0x03
#define si_ifferr     (1<<7)
#define si_itxffafull (1<<6)
#define si_itxffaem   (1<<5)
#define si_irxffafull (1<<4)
#define si_iext       (1<<3)
#define si_ipksent    (1<<2)
#define si_ipkvalid   (1<<1)
#define si_icrcerr    (1<<0)

/*---------------------------------------------*/
#define si_interrupt2 0x04
#define si_iswdet     (1<<7)
#define si_ipreaval   (1<<6)
#define si_ipreainval (1<<5)
#define si_irssi      (1<<4)
#define si_iwut       (1<<3)
#define si_ilbd       (1<<2)
#define si_ichiprdy   (1<<1)
#define si_ipor       (1<<0)
/*---------------------------------------------*/
#define si_inten1 0x05
/*---------------------------------------------*/
#define si_inten2 0x06
/*---------------------------------------------*/
#define si_mode01 0x07
#define si_swres       (1<<7)
#define si_enlbd       (1<<6)
#define si_enwt        (1<<5)
#define si_x32ksel     (1<<4)
#define si_txon        (1<<3)
#define si_rxon        (1<<2)
#define si_pllon       (1<<1)
#define si_xton        (1<<0)
/*---------------------------------------------*/
#define si_mode02 0x08
#define si_antdiv_mask (0x80|0x40|0x20)
#define si_rxmpk       (1<<4)
#define si_autotx      (1<<3)
#define si_enldm       (1<<2)
#define si_ffclrrx     (1<<1)
#define si_ffclrtx     (1<<0)
/*---------------------------------------------*/
#define si_30MHz_load 0x09
#define si_xtalshft    (1<<7)
#define si_xlc_mask    0x7f
/*---------------------------------------------*/
#define si_clkout 0x0a
#define si_clkt_mask    (0x20|0x10)
#define si_clkt_0       0
#define si_clkt_128     0x10
#define si_clkt_256     0x20
#define si_clkt_512     (0x20|0x10)
#define si_enlfc        (1<<3)
#define si_mclk_mask    (1|2|4)
#define si_mclk_30MHz   0x00
#define si_mclk_15MHz   0x01
#define si_mclk_10MHz   0x02
#define si_mclk_4MHz    0x03
#define si_mclk_3MHz    0x04
#define si_mclk_2MHz    0x05
#define si_mclk_1MHz    0x06
#define si_mclk_32768Hz 0x07

/*---------------------------------------------*/
#define si_GPIO0 0x0b
#define si_gpio0drv_mask 0xc0
#define si_gpio0drv_ll   0x00
#define si_gpio0drv_lh   0x40
#define si_gpio0drv_hl   0x80
#define si_gpio0drv_hh   0xc0
#define si_pup0          (1<<5)
#define si_gpio0fun_mask 0x1f
#define si_gpio0por      0x00
#define si_gpio0wut      0x01
#define si_gpio0lbd      0x02
#define si_gpio0ddin     0x03
#define si_gpio0extf     0x04
#define si_gpio0extr     0x05
#define si_gpio0extc     0x06
#define si_gpio0adc      0x07
#define si_gpio0atestnin 0x08
#define si_gpio0atestpin 0x09
#define si_gpio0ddo      0x0a
#define si_gpio0dtesto   0x0b
#define si_gpio0atestno  0x0c
#define si_gpio0atestpo  0x0d
#define si_gpio0refo     0x0e
#define si_gpio0datclko  0x0f
#define si_gpio0txdat    0x10
#define si_gpio0retrrq   0x11
#define si_gpio0txstate  0x12
#define si_gpio0txfifoaf 0x13
#define si_gpio0rxdat    0x14
#define si_gpio0rxstate  0x15
#define si_gpio0rxfifoaf 0x16
#define si_gpio0ant1sw   0x17
#define si_gpio0ant2sw   0x18
#define si_gpio0vpdet    0x19
#define si_gpio0ivpdet   0x1a
#define si_gpio0syncdet  0x1b
#define si_gpio0cca      0x1c
#define si_gpio0vdd      0x1d
#define si_gpio0gnd      0x1e
/*---------------------------------------------*/
#define si_GPIO1 0x0c
#define si_gpio1drv_mask 0xc0
#define si_gpio1drv_ll   0x00
#define si_gpio1drv_lh   0x40
#define si_gpio1drv_hl   0x80
#define si_gpio1drv_hh   0xc0
#define si_pup1          (1<<5)
#define si_gpio1fun_mask 0x1f
#define si_gpio1invpor   0x00
#define si_gpio1wut      0x01
#define si_gpio1lbd      0x02
#define si_gpio1ddin     0x03
#define si_gpio1extf     0x04
#define si_gpio1extr     0x05
#define si_gpio1extc     0x06
#define si_gpio1adc      0x07
#define si_gpio1atestnin 0x08
#define si_gpio1atestpin 0x09
#define si_gpio1ddo      0x0a
#define si_gpio1dtesto   0x0b
#define si_gpio1atestno  0x0c
#define si_gpio1atestpo  0x0d
#define si_gpio1refo     0x0e
#define si_gpio1datclko  0x0f
#define si_gpio1txdat    0x10
#define si_gpio1retrrq   0x11
#define si_gpio1txstate  0x12
#define si_gpio1txfifoaf 0x13
#define si_gpio1rxdat    0x14
#define si_gpio1rxstate  0x15
#define si_gpio1rxfifoaf 0x16
#define si_gpio1ant1sw   0x17
#define si_gpio1ant2sw   0x18
#define si_gpio1vpdet    0x19
#define si_gpio1ivpdet   0x1a
#define si_gpio1syncdet  0x1b
#define si_gpio1cca      0x1c
#define si_gpio1vdd      0x1d
#define si_gpio1gnd      0x1e
/*---------------------------------------------*/
#define si_GPIO2 0x0d
#define si_gpio1drv_mask 0xc0
#define si_gpio2drv_ll   0x00
#define si_gpio2drv_lh   0x40
#define si_gpio2drv_hl   0x80
#define si_gpio2drv_hh   0xc0
#define si_pup2          (1<<5)
#define si_gpio2fun_mask 0x1f
#define si_gpio2mclk     0x00
#define si_gpio2wut      0x01
#define si_gpio2lbd      0x02
#define si_gpio2ddin     0x03
#define si_gpio2extf     0x04
#define si_gpio2extr     0x05
#define si_gpio2extc     0x06
#define si_gpio2adc      0x07
#define si_gpio2atestnin 0x08
#define si_gpio2atestpin 0x09
#define si_gpio2ddo      0x0a
#define si_gpio2dtesto   0x0b
#define si_gpio2atestno  0x0c
#define si_gpio2atestpo  0x0d
#define si_gpio2refo     0x0e
#define si_gpio2datclko  0x0f
#define si_gpio2txdat    0x10
#define si_gpio2retrrq   0x11
#define si_gpio2txstate  0x12
#define si_gpio2txfifoaf 0x13
#define si_gpio2rxdat    0x14
#define si_gpio2rxstate  0x15
#define si_gpio2rxfifoaf 0x16
#define si_gpio2ant1sw   0x17
#define si_gpio2ant2sw   0x18
#define si_gpio2vpdet    0x19
#define si_gpio2ivpdet   0x1a
#define si_gpio2syncdet  0x1b
#define si_gpio2cca      0x1c
#define si_gpio2vdd      0x1d
#define si_gpio2gnd      0x1e
/*---------------------------------------------*/
#define si_ioport 0x0e
#define si_extist2     (1<<6)
#define si_extist1     (1<<5)
#define si_extist0     (1<<4)
#define si_itsdo       (1<<3)
#define si_dio2        (1<<2)
#define si_dio1        (1<<1)
#define si_dio0        (1<<0)
/*---------------------------------------------*/
#define si_adc_con 0x0f
#define si_adcstart     (1<<7)
#define si_adc_mask     ((1<<6)|(1<<5)|(1<<4))
#define si_adc_temp     ((0<<6)|(0<<5)|(0<<4))
#define si_adc_gpio0    ((0<<6)|(0<<5)|(1<<4))
#define si_adc_gpio1    ((0<<6)|(1<<5)|(0<<4))
#define si_adc_gpio2    ((0<<6)|(1<<5)|(1<<4))
#define si_adc_gpio01   ((1<<6)|(0<<5)|(0<<4))
#define si_adc_gpio12   ((1<<6)|(0<<5)|(1<<4))
#define si_adc_gpio02   ((1<<6)|(1<<5)|(0<<4))
#define si_adc_gnd      ((1<<6)|(1<<5)|(1<<4))
#define si_adcref_mask  ((1<<3)|(1<<2))
#define si_adcref_vbg   ((0<<3)|(0<<2))
#define si_adcref_vdd2  ((1<<3)|(0<<2))
#define si_adcref_vdd3  ((1<<3)|(1<<2))
#define si_adcgain_mask ((1<<1)|(1<<0))

/*---------------------------------------------*/
#define si_adc_ofsset 0x10
/*---------------------------------------------*/
#define si_adc 0x11
/*---------------------------------------------*/
#define si_temp_cal 0x12
#define si_tsrange_mask    ((1<<7)|(1<<6))
#define si_tsrange_64to64  ((0<<7)|(0<<6))
#define si_tsrange_64to192 ((0<<7)|(1<<6))
#define si_tsrange_0to128  ((1<<7)|(1<<6))
#define si_tsrange_40to216 ((1<<7)|(0<<6))
#define si_entsoffs        (1<<5)
#define si_entstrim        (1<<4)
#define si_tstrim_mask     0x07

/*---------------------------------------------*/
#define si_tvoffs  0x13
/*---------------------------------------------*/
#define si_wtr     0x14
/*---------------------------------------------*/
#define si_wtmh    0x15
/*---------------------------------------------*/
#define si_wtml    0x16
/*---------------------------------------------*/
#define si_wtvh    0x17
/*---------------------------------------------*/
#define si_wtvl    0x18
/*---------------------------------------------*/
#define si_ldc     0x19
/*---------------------------------------------*/
#define si_lbdt    0x1a
/*---------------------------------------------*/
#define si_vbat    0x1b
/*---------------------------------------------*/
#define si_iffbw   0x1c




/*---------------------------------------------*/
#define si_afc_loop_gearshift_override   0x1d
#define si_afcbd          (1<<7)
#define si_enafc          (1<<6)
#define si_afcgearh_mask  ((1<<5)|(1<<4)|(1<<3))
#define si_afcgearh_and   0x07
#define si_afcgearh_shift 3
#define si_1p5bypass      (1<<2)
#define si_matap          (1<<1)
#define si_ph0size        (1<<0)
/*---------------------------------------------*/
#define si_afc_timing   0x1e
#define si_swant_timer_mask  ((1<<7)|(1<<6))
#define si_swant_timer_and   3
#define si_swant_timer_shift 6
#define si_shwait_mask       ((1<<5)|(1<<4)|(1<<3))
#define si_shwait_and        0x07
#define si_shwait_shift      3
#define si_anwait_mask       ((1<<2)|(1<<1)|(1<<0))
#define si_anwait_and        0x07
#define si_anwait_shift      0
/*---------------------------------------------*/
#define si_cr_gearshift   0x1f
#define si_crfast_mask       ((1<<5)|(1<<4)|(1<<3))
#define si_crfast_and        0x07
#define si_crfast_shift      3
#define si_crslow_mask       ((1<<2)|(1<<1)|(1<<0))
#define si_crslow_and        0x07
#define si_crslow_shift      0
/*---------------------------------------------*/
#define si_rxosr   0x20
/*---------------------------------------------*/
#define si_crofs2   0x21
#define si_rxosrh_mask    0xe0
#define si_rxosrh_and     0x07
#define si_rxosrh_shift   5
#define si_skip2phth      (1<<4)
#define si_ncoffhl_mask   0x0f
#define si_ncoffhl_and    0x0f
#define si_ncoffhl_shift  0
/*---------------------------------------------*/
#define si_crofs1   0x22
#define si_ncofflh  0x22
/*---------------------------------------------*/
#define si_crofs0   0x23
#define si_ncoffll  0x23
/*---------------------------------------------*/
#define si_cr_timing_loop_gain0   0x24
#define si_rxncocomp     (1<<4)
#define si_cgainx2       (1<<3)
#define si_crgainh_mask  0x07
#define si_crgainh_and   0x07
#define si_crgainh_shift 0
/*---------------------------------------------*/
#define si_cr_timing_loop_gain1   0x25
#define si_crgainl        0x25
/*---------------------------------------------*/
#define si_rssi      0x26
/*---------------------------------------------*/
#define si_rssith    0x27
/*---------------------------------------------*/
#define si_adrssi1   0x28
/*---------------------------------------------*/
#define si_adrssi2   0x29
/*---------------------------------------------*/
#define si_afclim    0x2a
/*---------------------------------------------*/
#define si_afccorrh   0x2b
/*---------------------------------------------*/
#define si_ookcnt1     0x2c
#define si_afccorrl_mask   0xc0
#define si_afccorrl_and    0x03
#define si_afccorrl_shift  6
#define si_ookfrzen        (1<<5)
#define si_peakdeten       (1<<4)
#define si_madeten         (1<<3)
#define si_ookcnth_mask    0x03
#define si_ookcnth_and     0x03
#define si_ookcnth_shift   0

/*---------------------------------------------*/
#define si_ookcnt2     0x2d
#define si_ookcntl     0x2d
/*---------------------------------------------*/
#define si_slicer_peak_holder     0x2e
#define si_attack_mask  0x70
#define si_attack_and   0x07
#define si_attack_shift 4
#define si_decay_mask  0x0f
#define si_decay_and   0x0f
#define si_decay_shift 0
/*---------------------------------------------*/
#define si_data_access_contorl     0x30
#define si_enpacrx      (1<<7)
#define si_lsbfrst      (1<<6)
#define si_crcdonly     (1<<5)
#define si_skip2ph      (1<<4)
#define si_enpactx      (1<<3)
#define si_encrc        (1<<2)
#define si_crc_mask     0x03
#define si_crc_CCITT    0x00
#define si_crc16        0x01
#define si_crc_iec16    0x02
#define si_crc_biacheva 0x03
/*---------------------------------------------*/
#define si_ezmac_stat     0x31
#define si_rxcrc1   (1<<6)
#define si_pksrch   (1<<5)
#define si_pkrx     (1<<4)
#define si_pkvalid  (1<<3)
#define si_crcerror (1<<2)
#define si_pktx     (1<<1)
#define si_pksent   (1<<0)
/*---------------------------------------------*/
#define si_headcon1     0x32
#define si_bcen_mask    0xf0
#define si_bcen_and     0x0f
#define si_bcen_shift   4
#define si_hdch_mask    0x0f
#define si_hdch_and     0x0f
#define si_hdch_shift   0
/*---------------------------------------------*/
#define si_headcon2     0x33
#define si_skipsyn        (1<<7)
#define si_hdlen_mask     0x70
#define si_hdlen_none     0x00
#define si_hdlen_3        0x10
#define si_hdlen_32       0x20
#define si_hdlen_321      0x30
#define si_hdlen_3210     0x40
#define si_fixpklen          (1<<3)
#define si_synclen_mask   0x06
#define si_syncword3      0x00
#define si_syncword32     0x02
#define si_syncword321    0x04
#define si_syncword3210   0x06
#define si_prealenh_mask  (1<<0)
#define si_prealenh_and   1
#define si_prealenh_shift 0
/*---------------------------------------------*/
#define si_prealenl     0x34
/*---------------------------------------------*/
#define si_preadetcon     0x35
#define si_preath_mask       0xf8
#define si_preath_and        0x1f
#define si_preath_shift      3
#define si_rssi_offset_mask  0x07
#define si_rssi_offset_and   0x07
#define si_rssi_offset_shift 0
/*---------------------------------------------*/
#define si_synchh     0x36
#define si_sync3      0x36
/*---------------------------------------------*/
#define si_synchl     0x37
#define si_sync2      0x37
/*---------------------------------------------*/
#define si_synclh     0x38
#define si_sync1      0x38
/*---------------------------------------------*/
#define si_syncll     0x39
#define si_sync0      0x39
/*---------------------------------------------*/
#define si_txhdhh     0x3a
#define si_txhd3      0x3a
/*---------------------------------------------*/
#define si_txhdhl     0x3b
#define si_txhd2      0x3b
/*---------------------------------------------*/
#define si_txhdlh     0x3c
#define si_txhd1      0x3c
/*---------------------------------------------*/
#define si_txhdll     0x3d
#define si_txhd0      0x3d
/*---------------------------------------------*/
#define si_pklen      0x3e
/*---------------------------------------------*/
#define si_chhdhh     0x3f
#define si_chhd3      0x3f
/*---------------------------------------------*/
#define si_chhdhl     0x40
#define si_chhd2      0x40
/*---------------------------------------------*/
#define si_chhdlh     0x41
#define si_chhd1      0x41
/*---------------------------------------------*/
#define si_chhdll     0x42
#define si_chhd0      0x42
/*---------------------------------------------*/
#define si_hdenhh     0x43
#define si_hden3      0x43
/*---------------------------------------------*/
#define si_hdenhl     0x44
#define si_hden2      0x44
/*---------------------------------------------*/
#define si_hdenlh     0x45
#define si_hden1      0x45
/*---------------------------------------------*/
#define si_hdenll     0x46
#define si_hden0      0x46
/*---------------------------------------------*/
#define si_rxhdhh     0x47
#define si_rxhd3      0x47
/*---------------------------------------------*/
#define si_rxhdhl     0x48
#define si_rxhd2      0x48
/*---------------------------------------------*/
#define si_rxhdlh     0x49
#define si_rxhd1      0x49
/*---------------------------------------------*/
#define si_rxhdll     0x4a
#define si_rxhd0      0x4a
/*---------------------------------------------*/
#define si_rxplen     0x4b
/*---------------------------------------------*/
#define si_adc8       0x4f
/*---------------------------------------------*/
#define si_invalid_preamble_threshold 0x60
/*---------------------------------------------*/
#define si_crystal_por_cntl 0x62
#define si_pwst_mask      0xe0
#define si_pwst_lp        0x00
#define si_pwst_RDY       0x20
#define si_pwst_tune      0x60
#define si_pwst_tx        0x40
#define si_pwst_rx        0xe0
#define si_clkhyst        (1<<4)
#define si_enbias2x       (1<<3)
#define si_enamp2x        (1<<2)
#define si_bufovr         (1<<1)
#define si_enbuf          (1<<0)
/*---------------------------------------------*/
#define si_agc_override 0x69
#define si_sgin      (1<<6)
#define si_agcen     (1<<5)
#define si_lnagain   (1<<4)
#define si_pga_mask  0x07
#define si_pga_and   0x07
#define si_pga_shift 0
/*---------------------------------------------*/
#define si_txpower 0x6d
#define si_lnasw       (1<<3)
#define si_txpow_mask  0x07
#define si_txpow_and   0x07
#define si_txpow_shift 0

/*---------------------------------------------*/
#define si_txdrh 0x6e
/*---------------------------------------------*/
#define si_txdrl 0x6f
/*---------------------------------------------*/
#define si_modcon1 0x70
#define si_txdtrtscale   (1<<5)
#define si_enphpwdn      (1<<4)
#define si_manppol       (1<<3)
#define si_enmaninv      (1<<2)
#define si_enmanch       (1<<1)
#define si_enwhite       (1<<0)
/*---------------------------------------------*/
#define si_modcon2 0x71
#define si_trclk_mask    0xc0
#define si_trclk_none    0x00
#define si_trclk_gpio    0x40
#define si_trclk_sdo     0x80
#define si_trclk_nirq    0xc0
#define si_dtmod_mask    0x30
#define si_dtmod_gpio    0x00
#define si_dtmod_sdi     0x10
#define si_dtmod_fifo    0x20
#define si_dtmod_pn9     0x30
#define si_eninv         (1<<3)
#define si_fdh_mask      0x04
#define si_fdh_and       1
#define si_fdh_shift     2
#define si_modtyp_mask   0x03
#define si_modtyp_none   0x00
#define si_modtyp_ook    0x01
#define si_modtyp_fsk    0x02
#define si_modtyp_gfsk   0x03
/*---------------------------------------------*/
#define si_fdl 0x72
/*---------------------------------------------*/
#define si_fol 0x73
/*---------------------------------------------*/
#define si_foh 0x74
/*---------------------------------------------*/
#define si_fbsel 0x75
#define si_sbsel       (1<<6)
#define si_hbsel       (1<<5)
#define si_fb_mask     0x0f
#define si_fb_and      0x0f
#define si_fb_shift    0
/*---------------------------------------------*/
#define si_fch 0x76
/*---------------------------------------------*/
#define si_fcl 0x77
/*---------------------------------------------*/
#define si_fhch 0x79
/*---------------------------------------------*/
#define si_fhs 0x7a
/*---------------------------------------------*/
#define si_txafthr    0x7c
/*---------------------------------------------*/
#define si_txaethr    0x7d
/*---------------------------------------------*/
#define si_rxafthr    0x7e
/*---------------------------------------------*/
#define si_fifo      0x7f











#define si_get_iffbw_H2(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?\
	66:((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:\
	((x<=12.1)?52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?58:((x<=19.4)?59:((x<=21.4)?60:\
	((x<=23.9)?61:((x<=25.7)?62:((x<=28.2)?37:((x<=32.2)?38:((x<=34.7)?39:((x<=38.6)?43:\
	((x<=42.7)?44:((x<=47.7)?45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.2)?23:\
	((x<=75.2)?1:((x<=83.2)?2:((x<=90.0)?3:((x<=95.3)?4:((x<=112.1)?5:((x<=127.9)?6:((x<=137.9)?\
	7:((x<=142.8)?148:((x<=167.8)?149:((x<=181.1)?153:((x<=191.5)?150:((x<=208.4)?154:\
	((x<=225.1)?129:((x<=248.8)?130:((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?\
	137:((x<=420.2)?138:((x<=468.4)?139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:142)\
	)))))))))))))))))))))))))))))))))))))))))))))))))))))

	
#define si_get_iffbw_H210(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?66:\
	((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:((x<=12.1)?\
	52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?55:((x<=18.9)?33:((x<=21.6)?34:((x<=22.7)?35:\
	((x<=24.0)?36:((x<=28.2)?42:((x<=31.9)?31:((x<=34.7)?39:((x<=38.6)?43:((x<=42.7)?44:((x<=47.7)?\
	45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.2)?23:((x<=75.2)?1:((x<=83.2)?2:\
	((x<=90.0)?3:((x<=95.3)?4:((x<=112.1)?5:((x<=127.9)?6:((x<=137.9)?7:((x<=138.7)?10:((x<=154.2)?\
	11:((x<=168.0)?152:((x<=181.1)?153:((x<=208.4)?154:((x<=232.0)?155:((x<=256.0)?156:\
	((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?137:((x<=420.2)?138:\
	((x<=468.4)?139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:142))))))))))))))))))))))))))))))))))))))))))))))))))))))



	
#define si_get_iffbw_H10(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?66:\
	((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:((x<=12.1)?\
	52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?58:((x<=19.4)?59:((x<=21.4)?60:((x<=23.9)?61:\
	((x<=25.7)?62:((x<=28.2)?37:((x<=32.2)?38:((x<=34.7)?39:((x<=38.6)?43:((x<=42.7)?44:((x<=47.7)?\
	45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.4)?26:((x<=77.1)?27:((x<=85.1)?\
	28:((x<=95.3)?29:((x<=102.2)?30:((x<=115.6)?171:((x<=127.7)?172:((x<=142.8)?173:((x<=153.3)?\
	174:((x<=168.0)?152:((x<=181.1)?153:((x<=208.4)?154:((x<=232.0)?155:((x<=256.0)?156:\
	((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?137:((x<=420.2)?138:((x<=468.4)?\
	139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:((x<=620.7)?142:142))))))))))))))))))))))))))))))))))))))))))))))))))))))

#define si_ook_ndec(br) \
((br<=1000)?5:((br<=2000)?5:((br<=3000)?4:((br<=8000)?4:((br<=16000)?3:((br<=32000)?2:((br<=50000)?1:0)))))))


#define si_ook_filset(bw) \
((bw<=75.2)?0x01:((bw<=83.2)?0x02:((bw<=90)?0x03:((bw<=95.3)?0x04:((bw<=112.1)?0x05:((bw<=127.9)?\
0x06:((bw<=137.9)?0x07:((bw<=191.5)?0x80:((bw<=225.1)?0x81:((bw<=248.8)?0x82:((bw<=269.3)?0x83:\
((bw<=284.9)?0x84:((bw<=335.5)?0x88:((bw<=361.8)?0x89:((bw<=420.2)?0x8A:((bw<=468.4)?0x8B:\
((bw<=518.8)?0x8C:((bw<=577)?0x8D:0x8E))))))))))))))))))

#define si_ook_iffbw(Rb,bw) (si_ook_filset(bw)|(si_ook_ndec(Rb)<<4))

	
#define si_get_iffbw(x,H) ((H<=2)?si_get_iffbw_H2(x):((H<=10)?si_get_iffbw_H210(x):si_get_iffbw_H10(x)))

#define si_make_iffbw_lut_f_H10 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,58},\
{19.4,59},\
{21.4,60},\
{23.9,61},\
{25.7,62},\
{28.2,37},\
{32.2,38},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.4,26},\
{77.1,27},\
{85.1,28},\
{95.3,29},\
{102.2,30},\
{115.6,171},\
{127.7,172},\
{142.8,173},\
{153.3,174},\
{168.0,152},\
{181.1,153},\
{208.4,154},\
{232.0,155},\
{256.0,156},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
{620.7,142},\
}


#define si_make_iffbw_lut_f_H2_10 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,55},\
{18.9,33},\
{21.6,34},\
{22.7,35},\
{24.0,36},\
{28.2,42},\
{31.9,31},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.2,23},\
{75.2,1},\
{83.2,2},\
{90.0,3},\
{95.3,4},\
{112.1,5},\
{127.9,6},\
{137.9,7},\
{138.7,10},\
{154.2,11},\
{168.0,152},\
{181.1,153},\
{208.4,154},\
{232.0,155},\
{256.0,156},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
}

#define si_make_iffbw_lut_f_H2 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,58},\
{19.4,59},\
{21.4,60},\
{23.9,61},\
{25.7,62},\
{28.2,37},\
{32.2,38},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.2,23},\
{75.2,1},\
{83.2,2},\
{90.0,3},\
{95.3,4},\
{112.1,5},\
{127.9,6},\
{137.9,7},\
{142.8,148},\
{167.8,149},\
{181.1,153},\
{191.5,150},\
{208.4,154},\
{225.1,129},\
{248.8,130},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
}


#define si_make_iffbw_lut_i_H10 {\
{26,81},\
{28,82},\
{31,83},\
{32,84},\
{37,85},\
{49,65},\
{54,66},\
{59,67},\
{61,68},\
{72,69},\
{95,49},\
{106,50},\
{115,51},\
{121,52},\
{142,53},\
{162,54},\
{175,58},\
{194,59},\
{214,60},\
{239,61},\
{257,62},\
{282,37},\
{322,38},\
{347,39},\
{386,43},\
{427,44},\
{477,45},\
{512,46},\
{562,21},\
{641,22},\
{692,23},\
{752,1},\
{832,2},\
{900,3},\
{953,4},\
{1121,5},\
{1279,6},\
{1379,7},\
{1428,148},\
{1678,149},\
{1811,153},\
{1915,150},\
{2084,154},\
{2251,129},\
{2488,130},\
{2693,131},\
{2849,132},\
{3355,136},\
{3618,137},\
{4202,138},\
{4684,139},\
{5188,140},\
{5770,141},\
}


#define si_make_iffbw_lut_i_H2_10 {\
{26,81},\
{28,82},\
{31,83},\
{32,84},\
{37,85},\
{49,65},\
{54,66},\
{59,67},\
{61,68},\
{72,69},\
{95,49},\
{106,50},\
{115,51},\
{121,52},\
{142,53},\
{162,54},\
{175,55},\
{189,33},\
{216,34},\
{227,35},\
{240,36},\
{282,42},\
{319,31},\
{347,39},\
{386,43},\
{427,44},\
{477,45},\
{512,46},\
{562,21},\
{641,22},\
{692,23},\
{752,1},\
{832,2},\
{900,3},\
{953,4},\
{1121,5},\
{1279,6},\
{1379,7},\
{1387,10},\
{1542,11},\
{1680,152},\
{1811,153},\
{2084,154},\
{2320,155},\
{2560,156},\
{2693,131},\
{2849,132},\
{3355,136},\
{3618,137},\
{4202,138},\
{4684,139},\
{5188,140},\
{5770,141},\
}

#define si_make_iffbw_lut_i_H2 {\
{26,41},\
{28,42},\
{31,43},\
{32,44},\
{37,45},\
{49,33},\
{54,34},\
{59,35},\
{61,36},\
{72,37},\
{95,25},\
{106,26},\
{115,27},\
{121,28},\
{142,29},\
{162,30},\
{175,34},\
{194,35},\
{214,36},\
{239,37},\
{257,38},\
{282,21},\
{322,22},\
{347,23},\
{386,27},\
{427,28},\
{477,29},\
{512,30},\
{562,13},\
{641,14},\
{692,15},\
{752,1},\
{832,2},\
{900,3},\
{953,4},\
{1121,5},\
{1279,6},\
{1379,7},\
{1428,140},\
{1678,141},\
{1811,145},\
{1915,142},\
{2084,146},\
{2251,129},\
{2488,130},\
{2693,131},\
{2849,132},\
{3355,136},\
{3618,137},\
{4202,138},\
{4684,139},\
{5188,140},\
{5770,141},\
}



#define si4432_swreset() do{\
		REGW(0x07,0x80);\
		REGW(0x07,0x00);\
		REGW(0x07,0x01);\
		while(GET_nIRQ());\
		uint8_t dummy;\
		REGR(si_interrupt1,dummy);\
		REGR(si_interrupt2,dummy);\
		REGW(si_inten1,0);\
		REGW(si_inten2,0);\
	}while(0)


#define si4432_tune_base(fr) do{\
		REGW(0x75,(1<<6)|(((fr>=480000000.0)?1:0)<<5)|((uint8_t)(fr/10000000.0/((fr>=480000000.0)?2.0:1.0)-24.0)));\
		REGW(0x76,(((uint32_t)((fr/10000000.0/((fr>=480000000.0)?2.0:1.0)-24.0-\
			((uint32_t)(fr/10000000.0/((fr>=480000000.0)?2.0:1.0)-24.0)))*64000.0))>>8)&0xff);\
		REGW(0x77,((uint32_t)((fr/10000000.0/((fr>=480000000.0)?2.0:1.0)-24.0-\
			((uint32_t)(fr/10000000.0/((fr>=480000000.0)?2.0:1.0)-24.0)))*64000.0))&0xff);\
	}while(0)

#define si4432_setupgpio(g0,g1,g2,qoff) do{\
		REGW(si_GPIO0,g0);\
		REGW(si_GPIO1,g1);\
		REGW(si_GPIO2,g2);\
		REGW(0x09,qoff);\
		REGW(0x0a,0);\
	}while(0)


#define M_fd(de) (de*1000llu/625llu)
#define M_H(br,de,enmanch) (2llu*de/br/(1+enmanch))
#define M_BWmod(br,de,enmanch) (br*(1+enmanch)+2*de)
#define M_txdr(br) ((br<30000)?(br*(1llu<<21)/1000000llu):(br*(1llu<<16)/1000000llu))
#define M_slow(br) (br<30000)
#define M_iffbw(bandwidth,br,de,enmanch,mod) ((mod==si_modtyp_ook)?si_ook_iffbw(br,bandwidth):si_get_iffbw(bandwidth,M_H(br,de,enmanch)))
#define M_dwn3_bypass(bandwidth,br,de,enmanch,mod) ((M_iffbw(bandwidth,br,de,enmanch,mod)&0x80)>>7)
#define M_ndec_exp(bandwidth,br,de,enmanch,mod) ((M_iffbw(bandwidth,br,de,enmanch,mod)>>4)&0x7)
//#define M_Rb(br) (br/1000.0)

//#define M_rxosr(br,bandwidth,enmanch) (500.0*8.0*(1.0+2.0*M_dwn3_bypass(bandwidth))/((1llu<<M_ndec_exp(bandwidth))*M_Rb(br)*(1+enmanch)))
#define M_rxosr(br,bandwidth,de,enmanch,mod) (4000000llu*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch,mod))/\
	((1llu<<M_ndec_exp(bandwidth,br,de,enmanch,mod))*br*(1+enmanch)))

/*#define M_ncoff(bandwidth,br,de,enmanch) ((M_Rb(br)*(1llu<<(20+M_ndec_exp(bandwidth,br,de,enmanch))))/\
	(500.0*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch))))*/
#define M_ncoff(bandwidth,br,de,enmanch,mod) ((br*(1llu<<(20+M_ndec_exp(bandwidth,br,de,enmanch,mod))))/\
	(500000llu*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch,mod))))
#define M_crgain(br,de,bandwidth,enmanch,mod) (2+(1llu<<15)*(1+enmanch)*br/1000/M_rxosr(br,bandwidth,de,enmanch,mod)/de)

#define M_crgain_C(br,de,bandwidth,enmanch,mod) ((M_crgain(br,de,bandwidth,enmanch,mod)>0x07ff)?\
	(((uint32_t)M_crgain(br,de,bandwidth,enmanch,mod)>>1)|0x8000):\
	(uint32_t)M_crgain(br,de,bandwidth,enmanch,mod))
#define M_ncoff_C(bandwidth,br,de,enmanch,mod) (((uint32_t)M_ncoff(bandwidth,br,de,enmanch,mod)>=(1llu<<20))?\
	((1llu<<20)-1):M_ncoff(bandwidth,br,de,enmanch,mod))
#define M_ookcnt(bandwidth,br,de,enmanch,mod) ((uint32_t)(1500000llu/(br*(1+enmanch))))
#define M_afc_bw_lim(bandwidth,br,de,enmanch,mod) (1000llu*bandwidth/1250llu/(0+1))
#define M_afc_bw_lim_C(bandwidth,br,de,enmanch,mod) ((M_afc_bw_lim(bandwidth,br,de,enmanch,mod)>80)?80:M_afc_bw_lim(bandwidth,br,de,enmanch,mod))


#define si4432_setup_modem(br,deviation,bandwidth,enmanch,enafc,mod) do{\
		uint8_t buf;\
		REGR(0x70,buf);\
		if(enmanch)buf|=0x02;else buf&=~0x02;\
		buf&=~(1<<5);\
		buf|=(M_slow(br)<<5);\
		REGW(0x70,buf);\
		REGW(0x6f,M_txdr(br)&0xff);\
		REGW(0x6e,(M_txdr(br)>>8)&0xff);\
		REGW(0x1c,M_iffbw(bandwidth,br,deviation,enmanch,mod));\
		REGW(0x20,(uint32_t)M_rxosr(br,bandwidth,deviation,enmanch,mod)&0xff);\
		REGW(0x21,(((uint32_t)M_rxosr(br,bandwidth,deviation,enmanch,mod)>>3)&0xe0)|\
			(((uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch,mod)>>16)&0x0f));\
		REGW(0x22,((uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch,mod)>>8)&0xff);\
		REGW(0x23,(uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch,mod)&0xff);\
		REGW(0x24,(M_crgain_C(br,deviation,bandwidth,enmanch,mod)>>8)&0x0f);\
		REGW(0x25,M_crgain_C(br,deviation,bandwidth,enmanch,mod)&0xff);\
		REGW(0x1d,enafc?(/*si_afcbd|*/si_enafc|si_1p5bypass):0);\
		REGW(0x2a,enafc?M_afc_bw_lim_C(bandwidth,br,deviation,enmanch,mod):0x00);\
		if(mod==si_modtyp_ook)\
		{\
			REGW(0x2c,0x18|((M_ookcnt(bandwidth,br,deviation,enmanch,mod)>>8)&0xff)|(enmanch?0:0x20));\
			REGW(0x2d,M_ookcnt(bandwidth,br,deviation,enmanch,mod)&0xff);\
		}else{\
			REGR(0x71,buf);\
			buf&=~(1<<2);\
			buf|=((M_fd(deviation)>>(8-2))&(1<<2));\
			REGW(0x71,buf);\
			REGW(0x72,M_fd(deviation)&0xff);\
		}\
		REGW(si_modcon2,si_dtmod_fifo|mod);\
	}while(0)
	
#define si4432_hop(s,o) do{ uint8_t buf=s; REGW(si_fhs,buf);buf=o; REGW(si_fhch,buf); }while(0)



















#endif