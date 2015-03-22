$.fn.fadeSelfIn = function()
{
    return this.each(function()
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
}


$(document).ready(function()
{
    $(".as2 .commentTitle").click(function()
    {
        $(".as2 .myComments").slideToggle(650);
    });

    $(".as2 .step1").click(function()
    {
        $(".as2 .pic1").fadeSelfIn();
    });
    $(".as2 .step2").click(function()
    {
        $(".as2 .pic2").fadeSelfIn();
    });
    $(".as2 .step25").click(function()
    {
        $(".as2 .pic25").fadeSelfIn();
    });
    $(".as2 .screwups").click(function()
    {
        $(".as2 .picMis").fadeSelfIn();
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
                $(".as2 .choices").fadeIn(1000);
            });
        }
    });
});
