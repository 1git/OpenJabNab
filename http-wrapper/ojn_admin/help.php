<?php
require_once "include/common.php";
if(!defined('ADMIN_EMAIL'))
{
?>
This function is currently unavailable.
<?
}
else
{
$problemes = array(
0 => "Account creation or login problems",
1 => "Linking my rabbit to my account",
2 => "My rabbit does not connect",
99 => "Other",
);
if(count($_POST) && isset($_POST['mail']) && $_POST['mail'] != "")
{
	$message = "Mail : ".$_POST['mail']."\n";
	$message .= "Problem : ".$problemes[$_POST['probleme']]."\n";
	if(isset($_POST['serial']) && $_POST['serial'] != "")
		$message .= "Lapin : ".$_POST['serial']."\n";
	if(isset($_POST['user_name']) && $_POST['user_name'] != "")
		$message .= "Utilisateur : ".$_POST['user_name']."\n";
	$message .= strip_tags(stripslashes($_POST['desc']))."\n";
	$subject = "OpenJabNab Support Form";
	$headers = "From: openJabNab Admin <".ADMIN_EMAIL.">" . "\r\n" .
		   'Reply-To: ' . trim($_POST['mail']) . "\r\n" .
		   'X-Mailer: OJN' ;
	$to = "openJabNab Admin <".ADMIN_EMAIL.">";
	$answer = mail($to, $subject, $message, $headers);		
	$_SESSION['message'] = !$answer ? array('error' => "Une erreur est survenue lors de l'envoi du mail") : array('ok' => "Votre demande a &eacute;t&eacute; envoy&eacute;e &agrave; l'administrateur");
	header('Location: help.php');
	exit();
}
?>
<h1 >Support form</h1>
<br />
<?php
if(isset($_SESSION['message']) && empty($_GET)) {
	if(isset($_SESSION['message']['ok'])) { ?>
	<div class="ok_msg">
	<?php	echo $_SESSION['message']['ok'];
	} else { ?>
	<div class="error_msg">
	<?php	echo $_SESSION['message']['error'];
	}
	if(empty($_GET));
		unset($_SESSION['message']);
	echo "</div><br />";
}
?>
<form method="post">
<fieldset>
Email address : <input type="text" name="mail" value="">
<br />
<?php if(isset($_SESSION['token'])) { ?>
Username : <?php echo $_SESSION['login'] ?><input type="hidden" name="user_name" value="<?php echo $_SESSION['login'] ?>">
<?php } else { ?>
Username : <input type="text" name="user_name" value="">
<?php } ?>
<br />
My problem : <select name="probleme">
<?php foreach($problemes as $id => $probleme) { ?>
<option value="<?php echo $id ?>"><?php echo $probleme ?></option>
<?php } ?>
</select>
<br />
Serial number of the rabbit : <input type="text" name="serial" value="">
<br />
Description :<textarea name="desc" style="width: 300px; height: 150px"></textarea>

<br /><br /><input type="submit" value="Submit">
</form>
</fieldset>
<?
}
require_once "include/append.php";
?>
