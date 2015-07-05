(function ($) {
  Drupal.behaviors.jsWeightsPurple = {
    attach: function (context, settings) {
      var weight = settings.jsweights.purple;
      var newDiv = $('<div />').css('color', 'purple').html('I have a weight of ' + weight);
      $('#js-weights').append(newDiv);
    }
  };
})(jQuery);
