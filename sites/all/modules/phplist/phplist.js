$(document).ready(function()
{
  $('input').click(function()
  {
    var lid = this.name.substring(5, this.name.indexOf('['));
  	if (this.value == 0 && this.checked) {
  	  // Auto-select all the other options
	  $('#phplist-list-roles .form-checkbox').each(function()
	  {
	  	if (this.name.substring(5, this.name.indexOf('[')) == lid && this.value != 0) {
	  	  this.checked = true;
	  	}
	  });
    } else {
      if (this.value != 0 && !this.checked) $('#edit-lists' + lid + '-0').checked(false);
    
      if (this.value == 0) {
	    $('#phplist-list-roles .form-checkbox').each(function()
	    {
	  	  if (this.name.substring(5, this.name.indexOf('[')) == lid) {
	  	    this.checked = false;
	      }
	    });
	  }
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