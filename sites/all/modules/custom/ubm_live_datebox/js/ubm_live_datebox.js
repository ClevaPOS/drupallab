(function($)
{
  $(document).ready(function() {
    
    $(document).on("click",".clear-btn input", function(event){
      console.log('ok');
      region = $(this).parent().parent().attr('id');
      $('#' + region + ' input[type=text]').val('');
      event.preventDefault();
    });


 });

 
})(jQuery);