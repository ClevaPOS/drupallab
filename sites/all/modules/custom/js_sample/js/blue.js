(function($){
	Drupal.behaviors.jsWeightsBlue = {

		attach: function (context,settings){
			console.log(settings.jsweights.blue);
			var weight = settings.jsweights.blue;
			var newDiv = $('<div />').css('color', 'blue').html('I have a weight of ' + weight);
      		$('#js-weights').append(newDiv);
		}

	}


})(jQuery);