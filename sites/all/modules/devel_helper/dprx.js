Drupal.behaviors.initDevelHelper = function(){
	$('div.dprx-array, div.dprx-object').each( function(){
		$(this).click( function(e){
      // Toogle when the title is clicked
			if( $(e.target).hasClass('dprx-title') ){
			  // Open and close array and object value
				$(this).toggleClass('dprx-closed');
				// Cancel event bubbling since array and objects container are nested.
      }
			return false;	
		});
	});
};
