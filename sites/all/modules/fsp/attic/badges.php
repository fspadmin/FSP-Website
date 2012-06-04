<?php

$lines = array();

$lines[] = array(
'oid','email','cost','bill_fname','bill_lname','coupon','badge_1','badge_2','membership','items'
);

# Load orders
$res = db_query( "SELECT order_id as oid
  FROM {uc_orders}
  WHERE created > unix_timestamp('2011-10-01')
  LIMIT 1000
  ;" );

while ($oid = db_result($res)) {
  $l = array();
  $o = uc_order_load($oid);

  switch ($o->order_status) {
    case 'completed':
    case 'payment_received':
      break;
    default: 
      continue 2;
  }

  $l[] = $o->order_id;
  $l[] = $o->primary_email;
  $l[] = money_format('%.2n', abs($o->order_total));
  $l[] = $o->billing_first_name;
  $l[] = $o->billing_last_name;
  $l[] = ($c = $o->data['coupon']) ? $c : 'NOCOUPON';

  $u = user_load($o->uid);
  $l[] = ($b = $u->profile_badgename) ? $b : '';
  $l[] = ($b = $u->profile_badgeextra) ? $b : '';
  $l[] = get_what(array_values($u->roles));

  # Parse products
  $products = array();
  $items = array();
  foreach ($o->products as $p) {
    if (isset($p->data['module']) && $p->data['module'] == 'uc_product_kit') {
      $n = node_load($p->data['kit_id']);
      $products[$n->title] += 1;
    } else {
      $products[$p->model] += $p->qty;
    }
  }
  foreach ($products as $k=>$v) {
    $items[] = sprintf('(%s)%s', $v, $k);
  }

  $l[] = implode('|', $items);

  $lines[] = $l;
}

function get_what($roles = array()) {
  $what = 'other';
  foreach ($roles as $role) {
    if ($role == 'participant') return 'participant';
    if ($role == 'friend') $what = 'friend';
  }
  return $what;
}

$csv = fopen('/tmp/badges.csv', 'w');
foreach ($lines as $l)
  fputcsv($csv, $l);
fclose($csv);
