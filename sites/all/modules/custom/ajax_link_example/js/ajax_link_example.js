(function($){

	Drupal.behaviors.ajax_link_example = {

		detach: function (context, settings) {
	    // Do your js stuffs you want to do after your AJAX call.
		  console.log('test');
		  check = $('#ajax-link').html();
		  alert(check);

	  	}

  	}
 

	// Drupal.behaviors.ajax_link_example = {
	// 	attach:function(context, settings){
	// 		check = $('#ajax-link').val();
	// 	  	alert(settings);

	// 	}
	// }

   
})(jQuery);