$(document).ready(function()
{
    $(".as3 .perComs").click(function()
    {
        $(".as3 .coms").slideToggle(1000);
    });

    setSizes();

});

function setSizes()
{
    if (document.elementFromPoint(0, 0))
    {
        var maxPicWidth = Math.max($(".as3 .picture").width());
        var maxPicHeight = Math.max($(".as3 .picture").height());
        $(".as3 .picture").width(maxPicWidth)
            .height(maxPicHeight);

        $(".as3 .vid").css("max-width", $(".as3 .everything").width());
        return;
    }
    setTimeout(setSizes, 100);
}