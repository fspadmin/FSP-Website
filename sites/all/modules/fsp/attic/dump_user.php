<?php
$u=user_load(array('mail'=>'donnevanvdwatt@gmail.com'));
$u->pass='--- censored ---';
$cp=content_profile_load('rolodex', $u->uid);
print_r($u);
print_r($cp);
