(function ($) {
  Drupal.behaviors.jsWeightsGreen = {
    attach: function (context, settings) {
      var weight = settings.jsweights.green;
      var newDiv = $('<div></div>').css('color', 'green').html('I have a weight of ' + weight);
      $('#js-weights').append(newDiv);
    }
  };
})(jQuery);