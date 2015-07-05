(function($){
	Drupal.behaviors.jsWeightsBlack = {

		attach: function (context,settings){
			console.log(settings.jsweights.black);
			var weight = settings.jsweights.black;
			var newDiv = $('<div />').css('color', 'black').html('I have a weight of ' + weight);
      		$('#js-weights').append(newDiv);
		}

	}


})(jQuery);