$(document).ready(function()
        {
            $(".back").click(function()
                {
                    $(".aboutme").fadeOut();
                });
        });

$(window).load(function()
{
    $(".aboutme").fadeIn();
});

$( document ).on("pagecreate", ".homepage", function()
        {
            location.reload();
        });
