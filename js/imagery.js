$(document).ready( function() {
        $('.nav-tabs a').on("show.bs.tab", function(event) {
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
