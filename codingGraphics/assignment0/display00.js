$(document).ready(function()
{
    $(".as0 .response").click(function(e)
    {
        e.preventDefault();
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
            width: "100%",
            height: "100%"
        });

        $(".as0").height("100%");
    });
});
