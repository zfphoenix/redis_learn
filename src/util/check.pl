#!/usr/bin/perl -w
use strict;
use XML::Simple;
use POSIX;

my (@errors, $error_num);
my (%items);
$error_num = 0;

use constant PRE_ERROR  => 'Error NO.';
use constant COLOR_RED  => "\033[31m";
use constant COLOR_BLUE => "\033[32m";
use constant COLOR_YELL => "\033[33m";
use constant COLOR_END  => "\033[0m";

#arg 0, process meanwhile errors,
#arg 1, process the result errors
sub print_error {
    my ($arg) = @_;
    if ($arg eq 1 && $error_num eq 0) {
        print COLOR_BLUE . "Congratulations, no error found" . COLOR_END . "\n";
        exit 0;
    }
    foreach (@errors) {
        print COLOR_RED . $_ . COLOR_END . "\n";
    }
    if ($error_num ne 0) {
        exit 0;
    }
}

sub xml_existed {
    my ($xml_file) = @_;
    if (! -e $xml_file) {
        push @errors, PRE_ERROR . "$error_num  $xml_file not existed";
        $error_num += 1;
    } 
} 

sub column_exist {
    my ($h,$key,$value,$xml,$error,$id) = @_;
    if (exists $h->{$key}) {
        $$value = $h->{$key};
    }
    else {
        push @errors, PRE_ERROR . "$error_num $xml\'s column \'$key\' not found:Activity_ID=>$id";
        $error_num += 1;
        $$error += 1;
    }
}

sub check_date {
    my ($date, $id) = @_;
    my @arr = split('-', $date);
    if ($#arr ne 1) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated \'Activity_date\': ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    my @t1 = split('/', $arr[0]);
    my @t2 = split('/', $arr[1]);
    if ($#t1 ne $#t2) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated \'Activity_date\': ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    if ($#t1 ne 4 or $#t2 ne 4) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated \'Activity_date\': ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    foreach (0..4) {
        if ("" eq $t1[$_] || "" eq $t2[$_]) {
            push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated \'Activity_date\': ActivityID=>$id";
            $error_num += 1;
            return -1;
        }
    }
    my $time_stamp1 = POSIX::mktime(0, $t1[4], $t1[3], $t1[2], $t1[1] - 1, $t1[0] - 1900) or die "cannot mktime\n";
    my $time_stamp2 = POSIX::mktime(0, $t2[4], $t2[3], $t2[2], $t2[1] - 1, $t2[0] - 1900);
    if ($time_stamp1 >= $time_stamp2) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml wrong date \'Activity_date\': ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    return 0;
}

sub check_time {
    my ($time, $id) = @_;
    my @arr = split('-', $time);
    if ($#arr ne 1) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated"
            . "\'Activity_Time\' ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    my @d1 = split('/', $arr[0]);
    my @d2 = split('/', $arr[1]);
    if ($#d1 ne $#d2 or ($#d1 ne 1 or $#d2 ne 1)) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated"
            . "\'Activity_Time\' ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    my $flag = 0;
    if ($d1[0] > $d2[0]) {
        $flag = 1;
    }
    if ($d1[0] eq $d2[0]) {
        if ($d1[1] > $d2[1]) {
            $flag = 1;
        }
    }
    if ($flag eq 1) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml wrong time"
            . "\'Activity_Time\' ActivityID=>$id";
    }
    return 0;
}

sub check_day_times {
    my ($day, $id) = @_;
    my @arr = split('/', $day);
    if ($#arr ne 1) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated"
            . "\'Day_Times\' ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    if (!($arr[0] eq 0 || $arr[0] eq 1)) {
        push @errors, PRE_ERROR . "$error_num ActivityCentre.xml wrong day times" .
            " \'Day_Times\' ActivityID=>$id";
        $error_num += 1;
        return -1;
    }
    return 0;
}

sub check_reward {
    my ($reward, $id) = @_;
    if ($reward eq "0") {
        return 0;
    }
    my @arr = split(';', $reward);
    foreach (@arr) {
        my @temp = split('/', $_);
        if ($#temp ne 2) {
            push @errors, PRE_ERROR . "$error_num ActivityCentre.xml bad formated"
                . " \'Award\' ActivityId=>$id";
            $error_num += 1;
            return -1;
        }
        if (! exists $items{$temp[0]}) {
            push @errors, PRE_ERROR . "$error_num ActivityCentre.xml unknown item_id: $temp[0]"
                . " \'Award\' ActivityId=>$id";
            $error_num += 1;
            return -1;
        }
    }
    return 0;
}

&xml_existed("conf/Item.xml");
&xml_existed("conf/EquipmentItem.xml");
&xml_existed("conf/fairybasicinfo.xml");
&xml_existed("conf/ActivityCentre.xml");

&print_error(0);

print COLOR_YELL . "All xml file existed, begin check ... " . COLOR_END .
"\n";

my $xml_item = XMLin("conf/Item.xml");
my @temp = @{$xml_item->{'Items'}};
foreach (@temp) {
    $items{$_->{'ID'}} = 1;
}

my $xml_equip = XMLin("conf/EquipmentItem.xml");
@temp = @{$xml_equip->{'Equipment'}};
foreach (@temp) {
    $items{$_->{'ID'}} = 1;
}

my $xml_fairy = XMLin("conf/fairybasicinfo.xml");
@temp = @{$xml_fairy->{'fairy'}};
foreach (@temp) {
    $items{$_->{'ID'}} = 1;
}

#dragon heart
$items{$_} = 1 for (401000..401100);

@temp = keys %items;
print "All item num is $#temp\n";

my $xml_activity = XMLin("conf/ActivityCentre.xml");
@temp = @{$xml_activity->{'Activity'}};
foreach (@temp) {
    my ($activity_id,       $activity_type, $activity_show,     $last_activity_id);
    my ($time_type,         $offset_time,   $activity_date,     $activity_time);
    my ($special_time,      $award_time,    $level,             $vip_level);
    my ($time_qualify,      $day_times,     $activity_qualify1, $activity_qualify2);
    my ($award1,            $award2,        $ac_errors);
    $ac_errors = 0;
    $activity_id = 0;
    &column_exist($_, "ActivityID",
            \$activity_id,"ActivityCentre.xml",\$ac_errors, $activity_id);
    &column_exist($_, "Activity_Type", \$activity_type,
            "ActivityCentre.xml",\$ac_errors, $activity_id);
    &column_exist($_, "Activity_Show",
            \$activity_show,"ActivityCentre.xml",\$ac_errors, $activity_id);
    &column_exist($_, "Last_Activity_ID",
            \$last_activity_id,"ActivityCentre.xml",\$ac_errors, $activity_id);
    &column_exist($_, "Time_Type", \$time_type,"ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Offset_time",
            \$offset_time,"ActivityCentre.xml",\$ac_errors, $activity_id);
    &column_exist($_, "Activity_date", \$activity_date, "ActvityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Activity_Time", \$activity_time, "ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Special_Time", \$special_time, "ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Award_Time", \$award_time, "ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Level", \$level, "ActivityCentre.xml", \$ac_errors,
            $activity_id);
    &column_exist($_, "Vip_Level", \$vip_level, "ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Time_Qualification", \$time_qualify,
            "ActivityCentre.xml", \$ac_errors, $activity_id);
    &column_exist($_, "Day_Times", \$day_times, "ActivityCentre.xml",
            \$ac_errors, $activity_id);
    &column_exist($_, "Activity_Qualification", \$activity_qualify1,
            "ActvityCentre.xml", \$ac_errors, $activity_id);
    &column_exist($_, "Activity_Qualification2", \$activity_qualify2,
            \$ac_errors, $activity_id);
    &column_exist($_, "Award1", \$award1, "ActivityCentre.xml", \$ac_errors,
            $activity_id);
    &column_exist($_, "Award2", \$award2, "ActivityCentre.xml", \$ac_errors,
            $activity_id);

    if ($ac_errors ne 0) {
        next;
    }
    &check_date($activity_date, $activity_id);
    &check_time($activity_time, $activity_id);
    &check_day_times($day_times, $activity_id);
    &check_reward($award1, $activity_id);
    &check_reward($award2, $activity_id);
    print "$activity_id $level\n";
}

&print_error(1);


