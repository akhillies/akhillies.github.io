$(document).ready(function()
{
    $(".as0 .response").click(function(e)
    {
        $(".as0 .comments").slideToggle(500);
        $(".as0 .comments").animate(
        {
            opacity: 1 - $(".as0 .comments").css("opacity")
        }, 500);
    });
    $(".as0 .vid").click(function()
    {
        $(".as0 .vid").animate(
        {
            width: "75%",
            height: "75%"
        });
    });
});