var __sm_playAudioPool = {};
var __sm_ajaxCallBackPool = {};

function __audioFinishCallBack( p_audio_url )
{
    var t_audioUrl = p_audio_url;
    var t_callBack = __sm_playAudioPool[t_audioUrl];
    if( typeof( t_callBack ) !== "function" )
    {
        t_audioUrl = t_audioUrl.replace( "https://" , "" );
        t_callBack = __sm_playAudioPool[t_audioUrl];
    }

    if( typeof( t_callBack ) !== "function" )
    {
        return;
    }

    t_callBack( t_audioUrl );
    __sm_playAudioPool[ t_audioUrl ] = undefined;
}

function __ajaxSuccessCallBack( p_ajax_key, p_res )
{
    __ajaxCallBack( p_ajax_key + "success", p_ajax_key + "fial", p_res );
}

function __ajaxFialCallBack( p_ajax_key, p_res )
{
    __ajaxCallBack( p_ajax_key + "fial", p_ajax_key + "success", p_res );
}

function __ajaxCallBack( p_ajax_key_1, p_ajax_key_2, p_res )
{
    var t_ajaxKey1 = p_ajax_key_1;
    var t_ajaxKey2 = p_ajax_key_2;
    var t_callBack = __sm_ajaxCallBackPool[t_ajaxKey1];

    if( typeof( t_callBack ) !== "function" )
    {
        t_ajaxKey1 = t_ajaxKey1.replace( "https://" , "" );
        t_ajaxKey2 = t_ajaxKey2.replace( "https://" , "" );
        t_callBack = __sm_ajaxCallBackPool[t_ajaxKey1];
    }

    if( typeof( t_callBack ) !== "function" )
    {
        return;
    }

    t_callBack( p_res );
    __sm_ajaxCallBackPool[t_ajaxKey1] = undefined;
    __sm_ajaxCallBackPool[t_ajaxKey2] = undefined;
}


function goofypapaBack()
{
    window.location.href = "goofypapa://back";
}

function goofypapaLoadAudio( p_audio_url )
{
    window.location.href = "goofypapa://loadAudio," + p_audio_url;
}

function goofypapaPlayAudio( p_audio_url, p_finish_call_back )
{
    __sm_playAudioPool[p_audio_url] = p_finish_call_back;
    window.location.href = "goofypapa://playAudio," + p_audio_url + ",__audioFinishCallBack";
}

function goofypapaStopAudio( p_audio_url )
{
    __audioFinishCallBack( p_audio_url );
    window.location.href = "goofypapa://stopAudio," + p_audio_url;
}

function goofypapaStopAllAudio()
{
    for( var item in __sm_playAudioPool )
    {
        __audioFinishCallBack(item);
    }
    __sm_playAudioPool = {};

    window.location.href = "goofypapa://stopAllAudio";
}

function goofypapaStopAndPlayAudio( p_stop_audio_url, p_play_audio_url, p_finish_call_back )
{
    __audioFinishCallBack( p_stop_audio_url );
    __sm_playAudioPool[p_play_audio_url] = p_finish_call_back;
    window.location.href = "goofypapa://stopAudio," + p_stop_audio_url + ";playAudio," + p_play_audio_url + ",__audioFinishCallBack";
}

function goofypapaStopAllAndPlayAudio( p_audio_url, p_finish_call_back )
{

    for( var item in __sm_playAudioPool )
    {
        __audioFinishCallBack(item);
    }
    __sm_playAudioPool = {};

    __sm_playAudioPool[p_audio_url] = p_finish_call_back;
    window.location.href = "goofypapa://stopAllAudio;playAudio," + p_audio_url + ",__audioFinishCallBack";
}

function goofypapaPost( p_url, p_parameter, p_success_call_back, p_fial_call_back )
{
    goofypapaAjax( "POST", p_url, p_parameter, p_success_call_back, p_fial_call_back );
}

function goofypapaGet( p_url, p_parameter, p_success_call_back, p_fial_call_back )
{
    goofypapaAjax( "GET", p_url, p_parameter, p_success_call_back, p_fial_call_back );
}

function goofypapaAjax( p_ajax_type, p_url, p_parameter, p_success_call_back, p_fial_call_back )
{
    var t_type = "Post";

    switch( p_ajax_type.toLocaleUpperCase() )
    {
        case "post":
            t_type = "Post";
        break;
        case "get":
            t_type = "Get";
        break;
    }

    var t_parameter = "";
    for( var key in p_parameter )
    {
        t_parameter += "," + key + ":" + p_parameter[key];
    }

    var t_ajaxKey = t_type + p_url + t_parameter;
    var t_successCallBackKey = t_ajaxKey + "success";
    var t_fialCallBackKey = t_ajaxKey + "fial";

    if( typeof( __sm_ajaxCallBackPool[ t_successCallBackKey ] ) !== "undefined" || typeof( __sm_ajaxCallBackPool[ t_fialCallBackKey ] ) !== "undefined" )
    {
        return;
    }

    __sm_ajaxCallBackPool[ t_successCallBackKey ] = p_success_call_back;
    __sm_ajaxCallBackPool[ t_fialCallBackKey ] = p_fial_call_back;

    window.location.href = "goofypapa://" + t_type +  "," + p_url + ",window.__ajaxSuccessCallBack,window.__ajaxFialCallBack" + t_parameter;
}