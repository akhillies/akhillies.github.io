$(document).ready(function()
{
    $(".as2 .commentTitle").click(function()
    {
        $(".as2 .myComments").slideToggle(650);
    });

    $(".as2 .step1, .as2 .step2, .as2 .step25, as2 .screwups").click(function()
    {
        $(".as2 .titleChoice").fadeOut(1000, function()
        {
            $(".as2 .titleChoice").text("Click here to choose another milestone")
                .fadeIn(1000)
                .css("cursor", "pointer");
        });
        var pic = this;
        $(".as2 .choices").fadeOut(1000, function()
        {
            $(pic).fadeIn(1000);
        });

    });

    $(".as2 .titleChoice").click(function(e)
    {
        if ($(".as2 .titleChoice").text() == "Click here to choose another milestone")
        {
            $(".as2 .titleChoice").fadeOut(1000, function()
            {
                $(".as2 .titleChoice").text("Pick one of the following milestones I had in this project:")
                    .fadeIn(1000)
                    .css("cursor", "");
            });
            $(".as2 .pic").fadeOut(1000, function()
            {
                $(".as2 .choices").fadeIn(1500);
            });
        }
    });
});
