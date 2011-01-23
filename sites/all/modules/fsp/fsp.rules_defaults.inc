<?php

function fsp_rules_defaults(){
	$fsp_greeter_email_to = variable_get('fsp_greeter_email_to', NULL);
	$fsp_greeter_email_from = variable_get('fsp_greeter_email_from', NULL);
	$fsp_register_email_to = variable_get('fsp_register_email_to', NULL);

return array (
  'rules' => 
  array (
    'fsp_set_role' => 
    array (
      '#type' => 'rule',
      '#set' => 'event_node_insert',
      '#label' => 'On Participant creation set role to participant (SOI validated)',
      '#active' => 1,
      '#weight' => '0',
      '#categories' => 
      array (
      ),
      '#status' => 'custom',
      '#conditions' => 
      array (
        0 => 
        array (
          '#weight' => 0,
          '#info' => 
          array (
            'label' => 'Created Participant node (signed SOI)',
            'arguments' => 
            array (
              'node' => 
              array (
                'type' => 'node',
                'label' => 'Content',
              ),
            ),
            'module' => 'Node',
          ),
          '#name' => 'rules_condition_content_is_type',
          '#settings' => 
          array (
            'type' => 
            array (
              'participant' => 'participant',
            ),
            '#argument map' => 
            array (
              'node' => 'node',
            ),
          ),
          '#type' => 'condition',
        ),
      ),
      '#actions' => 
      array (
        0 => 
        array (
          '#type' => 'action',
          '#settings' => 
          array (
            'roles' => 
            array (
              0 => 30,
            ),
            '#argument map' => 
            array (
              'user' => 'author',
            ),
          ),
          '#name' => 'rules_action_user_addrole',
          '#info' => 
          array (
            'label' => 'Give Participant role',
            'arguments' => 
            array (
              'user' => 
              array (
                'type' => 'user',
                'label' => 'User whos roles should be changed',
              ),
            ),
            'module' => 'User',
          ),
          '#weight' => 0,
        ),
        1 => 
        array (
          '#weight' => 0,
          '#type' => 'action',
          '#settings' => 
          array (
            'roles' => 
            array (
              0 => 31,
            ),
            '#argument map' => 
            array (
              'user' => 'author',
            ),
          ),
          '#name' => 'rules_action_user_removerole',
          '#info' => 
          array (
            'label' => 'Remove Friend role (if present)',
            'arguments' => 
            array (
              'user' => 
              array (
                'type' => 'user',
                'label' => 'User whos roles should be changed',
              ),
            ),
            'module' => 'User',
          ),
        ),
      ),
      '#version' => 6003,
    ),
    'fsp_email_approver' => 
    array (
      '#type' => 'rule',
      '#set' => 'event_node_insert',
      '#label' => 'Send New User Email to Registration Admin',
      '#active' => 1,
      '#weight' => '0',
      '#categories' => 
      array (
      ),
      '#status' => 'custom',
      '#conditions' => 
      array (
        0 => 
        array (
          '#info' => 
          array (
            'label' => 'User has created a Rolodex entry',
            'label callback' => false,
            'arguments' => 
            array (
              'node' => 
              array (
                'type' => 'node',
                'label' => 'Content',
              ),
            ),
            'module' => 'Node',
          ),
          '#name' => 'rules_condition_content_is_type',
          '#settings' => 
          array (
            'type' => 
            array (
              'rolodex' => 'rolodex',
            ),
            '#argument map' => 
            array (
              'node' => 'node',
            ),
          ),
          '#type' => 'condition',
          '#weight' => 0,
        ),
        1 => 
        array (
          '#weight' => 0,
          '#negate' => 1,
          '#info' => 
          array (
            'label' => 'User is not already approved',
            'label callback' => false,
            'arguments' => 
            array (
              'user' => 
              array (
                'type' => 'user',
                'label' => 'User',
              ),
            ),
            'module' => 'User',
          ),
          '#name' => 'rules_condition_user_hasrole',
          '#type' => 'condition',
          '#settings' => 
          array (
            'roles' => 
            array (
              0 => 16,
            ),
            'operation' => 'OR',
            '#argument map' => 
            array (
              'user' => 'author',
            ),
          ),
        ),
      ),
      '#actions' => 
      array (
        0 => 
        array (
          '#weight' => 0,
          '#type' => 'action',
          '#settings' => 
          array (
            'to' => $fsp_register_email_to,
            'from' => '',
            'subject' => 'New User on FSP to be approved',
            'message' => 'A new user has signed up for the FSP website:

Approval Page: [node:site-url]/admin/user/accountapproval

User\'s name: [author:user]	
User\'s email address: [author:mail]
Edit: [node:site-url][author:account-edit]

[node:field_fsp_title-formatted] [node:field_fsp_first_name-formatted] [node:field_fsp_last_name-formatted]
[node:field_fsp_address_1-formatted]
[node:field_fsp_address_2-formatted]
[node:field_fsp_city-formatted], [node:field_fsp_state-formatted] [node:field_fsp_postal_code-formatted]
[node:field_fsp_country-formatted]

Referred: [node:field_fsp_referral-formatted]

Org: [node:field_fsp_organization-formatted]

Home: [node:field_fsp_home_phone-formatted]
Cell: [node:field_fsp_cell_phone-formatted]
Work: [node:field_fsp_work_phone-formatted]


[node:field_fsp_local_contact-raw]	Raw number value
[node:field_fsp_local_contact-formatted]
',
            '#eval input' => 
            array (
              'token_rules_input_evaluator' => 
              array (
                'message' => 
                array (
                  0 => 'node',
                  1 => 'author',
                ),
              ),
            ),
          ),
          '#name' => 'rules_action_mail',
          '#info' => 
          array (
            'label' => 'Send mail to Registration Admins',
            'label callback' => false,
            'module' => 'System',
            'eval input' => 
            array (
              0 => 'subject',
              1 => 'message',
              2 => 'from',
              3 => 'to',
            ),
          ),
        ),
      ),
      '#version' => 6003,
    ),
    'fsp_email_greeter' => 
    array (
      '#type' => 'rule',
      '#set' => 'event_user_update',
      '#label' => 'Send email to greeter when any site member is approved by approver.',
      '#active' => 1,
      '#weight' => '5',
      '#categories' => 
      array (
        0 => 'workflow',
        1 => 'approver',
        2 => 'registration',
        3 => 'participants',
      ),
      '#status' => 'custom',
      '#conditions' => 
      array (
        2 => 
        array (
          '#weight' => 0,
          '#type' => 'condition',
          '#settings' => 
          array (
            'code' => '$node = content_profile_load(\'rolodex\', $account->uid);
return $node->field_fsp_local_contact[0][\'value\'];',
            'vars' => 
            array (
              0 => 'account',
            ),
          ),
          '#name' => 'rules_condition_custom_php',
          '#info' => 
          array (
            'label' => 'Wants to be greeted',
            'label callback' => false,
            'module' => 'PHP',
            'eval input' => 
            array (
              0 => 'code',
            ),
          ),
        ),
        0 => 
        array (
          '#weight' => 1,
          '#negate' => 1,
          '#info' => 
          array (
            'label' => 'Used to be unapproved',
            'label callback' => false,
            'arguments' => 
            array (
              'user' => 
              array (
                'type' => 'user',
                'label' => 'User',
              ),
            ),
            'module' => 'User',
          ),
          '#name' => 'rules_condition_user_hasrole',
          '#type' => 'condition',
          '#settings' => 
          array (
            'roles' => 
            array (
              0 => 16,
            ),
            'operation' => 'OR',
            '#argument map' => 
            array (
              'user' => 'account_unchanged',
            ),
          ),
        ),
        1 => 
        array (
          '#type' => 'condition',
          '#settings' => 
          array (
            'roles' => 
            array (
              0 => 16,
            ),
            'operation' => 'OR',
            '#argument map' => 
            array (
              'user' => 'account',
            ),
          ),
          '#name' => 'rules_condition_user_hasrole',
          '#info' => 
          array (
            'label' => 'Is newly approved',
            'label callback' => false,
            'arguments' => 
            array (
              'user' => 
              array (
                'type' => 'user',
                'label' => 'User',
              ),
            ),
            'module' => 'User',
          ),
          '#weight' => 4,
        ),
      ),
      '#actions' => 
      array (
        0 => 
        array (
          '#weight' => 0,
          '#info' => 
          array (
            'label' => 'Email newly approved user\'s email to greeter.',
            'label callback' => false,
            'module' => 'System',
            'eval input' => 
            array (
              0 => 'subject',
              1 => 'message',
              2 => 'from',
              3 => 'to',
            ),
          ),
          '#name' => 'rules_action_mail',
          '#settings' => 
          array (
            'to' => $fsp_greeter_email_to,
						'from' => $fsp_greeter_email_from, 
						'subject' => 'Greet Me!   [account-name]',
						'message' => '<?php 
						$rolodex = content_profile_load( "rolodex", $account->uid );
						echo t(
							"User Status: @member_type\n" .
							"Honorific: @title\n" .
							"First Name: @first_name\n" .
							"Last Name: @last_name\n" .
							"\n" .
							"Address1: @address_1\n" .
							"Address2: @address_2\n" .
							"City: @city\n" .
							"State: @state\n" .
							"Country: @country\n" .
							"Postal Code: @postal_code\n" .
							"\n" . 
							"Phone(H): @home_phone\n" .
							"Phone(W): @work_phone\n" .
							"Phone(C): @cell_phone\n" .
							"Email: @email\n",
							array(
								"@member_type" => implode(", ", $account->roles),
								"@title" =>  $rolodex->field_fsp_title[0]["value"],
								"@first_name" => $rolodex->field_fsp_first_name[0]["value"],
								"@last_name" => $rolodex->field_fsp_last_name[0]["value"],

								"@address_1" => $rolodex->field_fsp_address_1[0]["value"],
								"@address_2" => $rolodex->field_fsp_address_2[0]["value"],
								"@city" => $rolodex->field_fsp_city[0]["value"],
								"@state" => $rolodex->field_fsp_state[0]["value"],
								"@postal_code" => $rolodex->field_fsp_postal_code[0]["value"],
								"@country" => $rolodex->field_fsp_country[0]["value"],

								"@home_phone" => $rolodex->field_fsp_home_phone[0]["value"],
								"@work_phone" => $rolodex->field_fsp_work_phone[0]["value"],
								"@cell_phone" => $rolodex->field_fsp_cell_phone[0]["value"],
								"@email" => $account->mail,

								));?>',
            '#eval input' => 
            array (
              'token_rules_input_evaluator' => 
              array (
                'message' => 
                array (
                  0 => 'account',
                ),
              ),
              'rules_input_evaluator_php' => 
              array (
                'message' => 
                array (
                  0 => 'account',
                ),
              ),
            ),

          ),
          '#type' => 'action',
        ),
      ),
      '#version' => 6004,
    ),
  ),
);
}
