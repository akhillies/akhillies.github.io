$(document).ready(function()
{
    $(".as1 .personal").click(function(e)
    {
        e.preventDefault();
        $(".as1 .comments").slideToggle(1000);
    });

    $(".as1 .all").click(function(e)
    {
        e.preventDefault();
        if ($(".as1 .all").text() == "View All Images")
        {
            $(".as1 .image").each(function(i, obj)
            {
                $(obj).slideDown(500,
                    function()
                    {
                        $(".as1 .all").html("Hide All Images");
                    })
                    .animate(
                    {
                        opacity: 1
                    }, 500);
            });
            $(".as1 .picTitle").css("boxShadow", "0px -4px 2px 1px black");
            $(".as1 .legendList").height(Math.max($(".as1 .sspl").find("img").height(), $(".as1 .ssdl").find("img").height()) - $(".as1 .legend").height());
        }
        else
        {
            $(".image").each(function(i, obj)
            {
                $(obj).animate(
                    {
                        opacity: 0
                    }, 500,
                    function()
                    {
                        $(".as1 .all").html("View All Images");
                    }
                )
                    .slideUp(500);
            });
            $(".as1 .picTitle").css("boxShadow", "0px 1px 2px 1px #252525");
        }
    });

    $(".as1 .ss-pl").click(function()
    {
        $(".as1 .sspl").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .ss-dl").click(function()
    {
        $(".as1 .ssdl").imageDisAppear();
        $(this).shadowToggle();
    });


    $(".as1 .ds-pl").click(function()
    {
        $(".as1 .dspl").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .dss-pl").click(function()
    {
        $(".as1 .dsspl").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .dss-pdl").click(function()
    {
        $(".as1 .dsspdl").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .toon").click(function()
    {
        $(".as1 .tn").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .twoSph").click(function()
    {
        $(".as1 .ts").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .multiSph").click(function()
    {
        $(".as1 .ms").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .cube").click(function()
    {
        $(".as1 .cb").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .multiCb").click(function()
    {
        $(".as1 .mc").imageDisAppear();
        $(this).shadowToggle();
    });

    $(".as1 .sphCub").click(function()
    {
        $(".as1 .sc").imageDisAppear();
        $(this).shadowToggle();
    });
});

$.fn.imageDisAppear = function()
{
    return this.each(function()
    {
        $(this).slideDown(500)
            .animate(
            {
                opacity: 1 - $(this).css("opacity")
            }, 500, function()
            {
                if ($(this).css("opacity") == 0)
                {
                    $(this).slideUp();
                }
            });
    });
}

$.fn.shadowToggle = function()
{
    return this.each(function()
    {
        $(this).css("boxShadow", "0px " + (-3 - $(this).css("boxShadow").split("px ")[1]) + "px 2px 1px " + ["#252525", "black"][$(this).css("boxShadow").split("px ")[1] % 2]);
    });
}