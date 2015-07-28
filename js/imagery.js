$(document).ready( function() {
        $('.nav-tabs a').on("show.bs.tab", function(event) {
            console.log('#' + event.target);
            if(history.pushState)
            {
                history.pushState(null, null, '#' + event.target);
            }
            else
            {
                location.hash = '#' + event.target;
            }
        });
    });
