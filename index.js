$(document).ready(function()
{
    $(".homepage .aboutBtn").click(function()
    {
        $(".homepage .bio").slideToggle(600);
    });
    $(".homepage .newpage").click(function()
    {
        $(".homepage").fadeOut();
    });
});

$(window).load(function()
{
    $(".homepage .myName").slideDown(1500);
    $(".homepage .bgImage").animate(
        {
            opacity: 0.15,
            width: "100%",
            height: "100%"
        },
        1500);
    $(".homepage .logo").animate(
        {
            opacity: 1,
            width: "100%",
            height: "100%",
            margin: 0
        },
        1500);
    $(".homepage .info").fadeIn(2000);
    $(".homepage .mePhoto").fadeIn(2000);
    $(".homepage .bottomBar").animate(
        {
            top: "95%"
        },
        1000);
    $(".homepage .bottomText").animate(
        {
            top: "95%"
        },
        1500);
    $(".homepage .workList").css("margin-top", $(".homepage .titleBtn").css("height"));
});

$( document ).on( "pageinit", ".aboutPage", function( event ) {
  alert( "This page was just enhanced by jQuery Mobile!" );
});
