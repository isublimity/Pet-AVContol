<?php
class AVDecoder
{
    public $data=[];

    private $decode=
        [
            'INPUT MAIN'=>[
                '0000'=>'IN1 BD/DVD',
                '0001'=>'IN2 VCR',
                '0010'=>'IN3 CBL',
                '0011'=>'IN4 NULL',
                '0100'=>'IN5 GAME',
                '0101'=>'IN6 TV',
                '0110'=>'IN7 TUNER',
                '0111'=>'IN8 INSIGNIA',
                '1000'=>'IN9 AUX',
                '1001'=>'IN10 NET USB',
                '1010'=>'IN12',
                '1110'=>'IN13',
                '1111'=>'IN14 NULL',
                '1011'=>'IN_A', // IF?
                '1100'=>'IN_B', // IF?
            ]
        ];
    private $cmd=[

    '001'=>[
        'REC I/O'=>20,
        'NA19'=>19,
        'BASS'=>[15,16,17,18],
        'NA10_001'=>10,
        'NA11_001'=>11,
        'NA12_001'=>12,
        'NA13_001'=>13,
        'NA14_001'=>14,
        'NA15_001'=>15,
        'L/R VOL IN'=>[8,9],
        'INPUT SUB'=>[4,5,6,7],
        'INPUT MAIN'=>[0,1,2,4]
    ],
    '010'=>[
        'NA20_010'=>20,
        'NA19_010'=>19,
        'TREBLE'=>[15,16,17,18],
        'FLch_Vol95'=>[8,9,10,11,12,13,14],
        'FLch_Vol42'=>[0,1,2,3,4,5,6],
    ],
    '011'=>[
        'REC MS'=>20,
        'REC5'=>19,
        'REC4'=>18,
        'REC3'=>17,
        'REC2'=>16,
        'REC1'=>15,
        'FRch_Vol95'=>[8,9,10,11,12,13,14],
        'FRch_Vol42'=>[0,1,2,3,4,5,6],
    ],

    '100'=>[
        'ATT'=>[19,20],
        'C/SW V'=>18,
        'SWch_Vol'=>[9,10,11,12,13,14,15,16,17],
        'Cch_Vol'=>[0,1,2,3,4,5,6,7,8],
    ],

    '101'=>[
        'ZERO101'=>20,
        'SL/SR Vol IN'=>[18,19],
        'SRch_Vol'=>[9,10,11,12,13,14,15,16,17],
        'SLch_Vol'=>[0,1,2,3,4,5,6,7,8],
    ],

    '110'=>[
        'ZERO110'=>20,
        'MIX'=>19,
        'BYPASS'=>18,
        'NULL110'=>[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
    ],

    '111'=>[
        'ZERO111'=>20,
        'NULL111'=>[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19],

    ],
    '000'=>[
        'ZERO000'=>20,
        'NULL000'=>[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19],

    ],

    ];
    public function __construct($filePath,$columnSet)
    {
        $this->data=$this->readLoginCSV($filePath,$columnSet);
    }

    private function readLoginCSV($file,$columnSet)
    {
        $csvFile = file($file);
        $data = [];
        $num=0;
        foreach ($csvFile as $line) {
            $out=[];
            $r= str_getcsv($line);
            $num++;
            if ($num==1) continue;
            $out['time']=floatval($r[0]);
            foreach ($columnSet as $key=>$colNumber) {
                if (intval($colNumber) > 0 && $colNumber !== null) {
                    $out[$key] = intval($r[$colNumber]);
                }
            }
            $data[]=$out;
        }
        return $data;
    }


    private function r2a_bitsToKeys($keys,$bits)
    {
        $out=[];
        foreach ($keys as $key=>$pos)
        {
            if (is_array($pos)) {
                $out[$key]=substr($bits,$pos[0],sizeof($pos));

            } else {
                $out[$key]=intval(substr($bits,$pos,1));
            }
        }
        return $out;
    }
    private function r2a_001($item)
    {

        $command=substr($item['b'],-3);

        if (empty($this->cmd[$command])) {
            echo 'Cant find commmand:'.$command."\n";
            exit;
        }
        $out=$this->r2a_bitsToKeys($this->cmd[$command],$item['b']);
        $out['time']=$item['t'];
        $out['text']='';
        $out['bits']=$item['b'];

        // TREBLE
        // INPUT MAIN
        // BASS
        return $out;
    }
    public function r2a($filterRangeTime=null)
    {

        // Пакет 24 бита по 8 пачек, то есть 1 байт = 24 бита (за 1 байт будем считать пакет информации отправленный на один адрес, так проще)
        // Всего 8 адресов, адрес передается в последних 3-х битах каждого пакета.
        // 1. Бит данных (Data) должен быть выставлен до появления сигнала тактирования (Clock)
        // 2. Бит данных не должен залезать на вторую половину сигнала тактирования
        // 3. Обязательное формирования сигнала защелки в виде High на Data после 24 бита иначе данные не прочитаются.
        // 4. Данные читаются по нарастающему фронту.
        // 5. Защелка читается по спадающему фронту.
        // Ну в принципе там все видно, 24 бита, данные читаются по фронту клока, защелка по спаду.
        // В настройках анализатора ставьте протокол SPI, 24 Bits per Transfer.


        $out=[];
        $counter=0;
        echo "\n\n";
        echo "Count lines:".sizeof($this->data)."\n\n";
        $startBitFind=false;
        $clocks='00';
        $data='00';
        $string='00';
        $preData=null;
        $preClock=null;
        $outBitString='';
        foreach ($this->data as $imp) // impulse
        {
            $time=$imp['time'];
            $d=$imp['data'];
            $c=$imp['clock'];

            // $str=strval($d).''.strval($c);
            // $string.=$str;
            // $sw=(substr($string,-10));

            // Данные читаются по нарастающему фронту
            if ($preClock===0 && $c===1)
            {
                $outBitString.=strval($d);
                if (strlen($outBitString)>23) {
                    // по 8 бит
                   $out[]=['t'=> round($time,4),'b'=>$outBitString];
                   $outBitString='';
                }
            }
            if ($filterRangeTime!==null) {
                if (
                        $time>=$filterRangeTime['up'] &&
                        $time<=floatval($filterRangeTime['to'])
                    )
                    {
                    // $t_p=($startBitFind?$startBitFind:' ').' '.$sw;
                    }
            }
            $preClock=$c;
            $counter++;
            // if ($counter>5000000) break;
        }
        // --------
        $columns=['time'=>1,'note'=>1,'text'=>1];
        foreach ($this->cmd as $cmd_num=>$rows)
        {
            foreach ($rows as $name=>$pins)
            {
                if (isset($columns[$name])) {
                    echo 'ERROR in name colum:'.$name.' need rename';
                    exit;
                }
                $columns[$name]=1;
            }

        }
        $columns['bits']=1;
        $result=[];
        foreach ($out as $item)
        {
            $ll=[];
            $r=$this->r2a_001($item);
            foreach ($columns as $col=>$tmp)
            {
                $ll[$col]=(isset($r[$col])?$r[$col]:'');
            }
            $result[]=$ll;
        }
        return $result;
    }
}

// -----------
ini_set("memory_limit","856M");
$options = getopt("",['file:','range::','out:']);
function exitMessage($text='')
{
    echo "Error:$text\n\tUse:\n\tr2a_decode.php --file='~/home/path/file.csv' --out='/tmp/file1.tsv'\n";
    exit;
}
if (empty($options['out']))
{
    exitMessage('Not --out');
}
if (empty($options['file']) || !is_file(@$options['file']))
{
    exitMessage('Not set --file or not find file.');
}


function writeToExcell($fileOut,$r)
{

    $out = fopen($fileOut, 'w');

    $excel_1='<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet; charset=utf-8" />
</head>
<body>
<style>
.num {
  mso-number-format:General;
}
.text {
  mso-number-format:"\@"
}
</style>
<table>
';
    $excel_2=' </table> </body> </html> ';

    fputs($out,$excel_1);
    fputs($out,"<tr><td>\n".implode("</td>\t<td x:str class='text'>",array_keys($r[0]))."</td></tr>\n\n");
// fputcsv($out, array_keys($r[0]),"\t");
    foreach ($r as $row) {
        // fputcsv($out, $row,"\t");
        fputs($out,"<tr><td>\n".implode("</td>\t<td class='text'>",$row)."</td></tr>\n\n");
    }
    fputs($out,$excel_2);
    fclose($out);

}
// ---------------------
$filePath=$options['file'];
$fileOut=$options['out'];
if (is_file($fileOut)) unlink($fileOut);

// Debug range
$filterRange=null;
if (isset($options['range']) && strpos($options['range'],":")) {
    list($u,$t)=explode(':',$options['range']);
    $filterRange['up']=floatval($u);
    $filterRange['to']=floatval($t);
}


// Result file
$columnSet=
    [
        'data'=>1,
        'clock'=>2,
        'reset'=>null,
        'cs'=>null
    ];


$f=new AVDecoder($filePath,$columnSet);
$r=$f->r2a($filterRange);
// Store