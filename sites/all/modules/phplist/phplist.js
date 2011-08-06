$(document).ready(function()
{
  $('#edit-phplist-sync').click(function()
  {
	if (this.checked) {
	  // Can enable next checkbox
	  $('#edit-phplist-sync-reset').removeAttr('disabled');
	}
	else {
	  $('#edit-phplist-sync-reset').attr('disabled', true);
	  $('#edit-phplist-sync-partial').attr('disabled', true);
	  $('#edit-phplist-sync-reset').checked(false);
	  $('#edit-phplist-sync-partial').checked(false);
	}
  });
  
  $('#edit-phplist-sync-reset').click(function()
  {
	if (this.checked) {
	  // Can enable next checkbox
	  $('#edit-phplist-sync-partial').removeAttr('disabled');
	}
	else {
	  $('#edit-phplist-sync-partial').attr('disabled', true);
	  $('#edit-phplist-sync-partial').checked(false);
	}
  });
  
  $('#edit-phplist-subscribe-on-register').click(function()
  {
	 if (this.checked) {
		// Can enable next checkbox
		$('#edit-phplist-descriptions-registerpage').removeAttr('disabled');
	 }
	 else {
		$('#edit-phplist-descriptions-registerpage').attr('disabled', true);
		$('#edit-phplist-descriptions-registerpage').checked(false);
	 }
  });
});


jQuery.fn.checked = function()
{
  selectornot = arguments[0];
  this.each(function() 
  {
	this.checked = selectornot;
  });
}