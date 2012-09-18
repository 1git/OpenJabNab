<?php
if(!file_exists("include/common.php"))
	header('Location: install.php');
require_once "include/common.php";

if(isset($_GET['logout'])) {
	unset($_SESSION['bunny']);
	unset($_SESSION['bunny_name']);
	unset($_SESSION['ztamp']);
	unset($_SESSION['ztamp_name']);
	unset($_SESSION['login']);
	$ojnAPI->SetToken('');
	unset($_SESSION['token']);
	header("Location: index.php");
}
if(!empty($_POST['login']) && !empty($_POST['password'])) {
	$r = $ojnAPI->loginAccount($_POST['login'], $_POST['password']);
	if(!strpos($r,"AD_")) {
		$_SESSION['login'] = $_POST['login'];
		$ojnAPI->setToken($r);
	} else {
		$_SESSION['message']['error'] = "Bad authentification.";
	}
	session_write_close();
	header("Location: index.php");
}
?>
<?php
if(isset($_SESSION['message']) && empty($_GET)) {
	if(isset($_SESSION['message']['ok'])) { ?>
	<div class="ok_msg">
	<?php	echo $_SESSION['message']['ok'];
	} else { ?>
	<div class="error_msg">
	<?php	echo $_SESSION['message']['error'];
	}
	if(empty($_GET))
		unset($_SESSION['message']);
	echo "</div>";
}
?>
<div class="three_cols">
      <h1 id="accueil">Home</h1>
      <p>Welcome to the OpenJabNab configuration page. From here you can enable or disable, configure and schedule the features that manage your Nabaztag bunny.</p>
</div>

<div class="three_cols">
<?php
if(isset($_SESSION['token'])) {
?>
<h1>Logout</h1>
Click on the link to logout: <a href="index.php?logout">Logout</a>
<?php
} else {
?>
      <h1 id="tutorial">Login</h1>
      <form method="post">
	<dl>
	<dt>Login</dt>
	<dd><input type="text" name="login"></dd>
	<dt>Password</dt>
	<dd><input type="password" name="password"></dd>
	</dl>
	<input type="submit" value="Login">
	</form>
<?php
}
?>
</div>

<div class="three_cols">
      <h1 id="tutorial">New account</h1>
<p>If you want to register your rabbit to this server, you need to create an account. <a href="register.php">Register here</a>.</p>
</div>
<?php
require_once("include/append.php");
?>
