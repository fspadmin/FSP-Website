<?php
switch ($vars['title']) {

	case 'rolodex':
		$title = 'Private information';
		break;

	case 'participant':
		$title = 'Participant details';
		break;

}

$vars['title'] = $title;
