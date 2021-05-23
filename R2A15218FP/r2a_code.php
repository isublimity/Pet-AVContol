<?php
class R2A15218FP_Command
{

    // INPUTMAIN
    // TUNER= 0110 , // IN7
    // PORT = 1001 , // IN10 [INL10/RECL4 & INR10/RECR4]
    // USB  = 0000 ,
    // TV   = 0101 , // IN6
    // VCR  = 0001 , // IN2
    // CBL  = 0010 , // IN3
    // GAME = 0100 , // IN5
    // AUX  = 1000
    // 001
    private $RECIO=false;
    private $BASS=0;
    private $LRVOLInput=0;
    private $INPUTSUB=  0 ; // Always = 1111
    private $INPUTMAIN=0;   // PORT = 1001 , USB = 0000 , TV = 0101 ,  VCR = 0001, CBL = 0010, GAME = 0100, AUX = 1000
    private $NA_D19a = 1;   // Force set 1 ?WTF?
    private $NA_D15a = 1;   // Force set 1 ?WTF?
    // 010


    private function dumpBin($b)
    {
        $str='';
        for ($i=23;$i>=0;$i--) {
            $u =( $b & ( 1 << $i ) );
            $str.= ($u?'1':'0');
        }
        echo implode(' ',str_split($str,8))." \n";
        echo "\n";
    }
    public function nbit($number, $n) { return ($number >> $n-1) & 1;}
    public function dump()
    {
        $mask=0xFFFFF;

        $b = ( 0x0F << 24 ); // Поставить 25-28й бит - для отрисовки
        $b |=  ( 1 << 0 ) ; // Ставим 10 бит
        $b |=  ( 1 << 1 ) ; // Ставим 10 бит
        $b |=  ( 1 << 2 ) ; // Ставим 10 бит
        // $b |=  ( 1 << 3 ) ; // Ставим 10 бит
//        $b |=  ( 1 << 23 ) ;


        $address = 0x0F & $b;

        var_dump($address);

        // $b &= $mask;
        // value &= ~mask;

        // number &= ~(1 << x); // UN SET BIT


        // $b = $b;

        $this->dumpBin($b);



        exit;
        // echo implode(' ',str_split($this->dec2bin($p3),8))." \n";
        for ( $i=23; $i>=0; $i--) {
            echo $i.' '.$this->nbit($b,$i) ."\n";
        }
    }
}
$n=new R2A15218FP_Command();
$n->dump();

//vol_l = ((87 - vol_l)  << 1);
//byte sum_in_gain = (in << 5) + (gain << 1);
//  writeWire(INPUT_GAIN, sum_in_gain);


//byte sum_st_md = (stereo << 6) + (mode << 4);
//  writeWire(STEREO_MODE, sum_st_md);

//if(bass >= 0){bass = ((bass + 0b1000) << 4);}
//if(bass < 0){bass = (abs(bass) << 4);}
//if(treb >= 0){treb = treb + 0b1000;}
//if(treb < 0){treb = abs(treb);}
//byte sum_bass_treb = bass + treb;