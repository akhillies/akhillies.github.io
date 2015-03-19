$(window).load(function()
{
    $(".aboutme").fadeIn();
});

$( document ).on("pagecreate", ".homepage", function()
        {
            location.reload();
        });
