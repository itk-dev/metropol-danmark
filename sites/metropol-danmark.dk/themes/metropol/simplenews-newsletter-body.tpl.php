<?php
// $Id: simplenews-newsletter-body.tpl.php,v 1.1.2.4 2009/01/02 11:59:33 sutharsan Exp $

/**
 * @file
 * Default theme implementation to format the simplenews newsletter body.
 *
 * Copy this file in your theme directory to create a custom themed body.
 * Rename it to simplenews-newsletter-body--<tid>.tpl.php to override it for a 
 * newsletter using the newsletter term's id.
 *
 * Available variables:
 * - node: Newsletter node object
 * - $body: Newsletter body (formatted as plain text or HTML)
 * - $title: Node title
 * - $language: Language object
 *
 * @see template_preprocess_simplenews_newsletter_body()
 */
 
 //$image_folder = $base_url . '/sites/metropol-danmark.dk/themes/metropol-danmark/images/nyhedsbrev';
 $image_folder = 'http://metropol-danmark.dk/sites/metropol-danmark.dk/themes/metropol/images/nyhedsbrev';
 $month = $node->field_nyhedsbrev_dato[0]['view'];
 $main_text = $node->content['body']['#value'];
 $main_image_path = $node->field_nyhedsbrev_image[0]['filepath'];
 $main_image = theme('imagecache', 'nyhedsbrev', $main_image_path, NULL, NULL, array('style' => 'margin-top:10px;'));
 $vidste_nid = $node->field_nyhedsbrev_vidste_du[0]['nid'];
 $vidste = node_load($vidste_nid);
 $vidst_du_text = $vidste->field_vidste_du_text[0]['value'];
?>
<table style="background-color:#cccccc": width="100%" border="0" cellspacing="0" cellpadding="0" class="bg1">
    <tr>
      <td align="center">
        <table style="background-color:#ffffff; font-family:Arial,Helvetica,sans-serif;" width="550" border="0" cellspacing="0" cellpadding="0" class="bg2">
		<td colspan="2" width="550" style="text-align:center; padding:0; font-family:arial; font-size:11px" align="center" valign="top"><a text-align:center;" href="http://metropol-danmark.dk/nyhedsbreve">Har du problemer med at læse nyhedsbrevet, så klik her</a></td>
          <tr>
            <td style="padding:0;" align="right" valign="top"><img src="<?php print $image_folder ?>/header-left.gif"
            alt="Header" width="360" height="102" /></td>
            <td align="left" valign="top" width="190"><img src="<?php print $image_folder; ?>/header-right.gif"
            alt="Header" width="190" height="60"/>
            <h2 style="font-family:Arial; font-size:12px; background:#79a800; width:101px; line-height:28px; text-align:center; color:#ffffff;  margin-left:79px; margin-top:0;"><?php print $month; ?></h2></td>
          </tr>
			<tr>
            <td colspan="2">
            <img src="<?php print $image_folder; ?>/gradient.gif"
            width="550" height="10" />
            </td>
            </tr>
          <tr>
            <td valign="top" colspan="2">
              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td style="padding-left:20px; padding-bottom:20px; font-size:13px;" width="360" valign="top" class="mainbar" align="left">
                  <?php print $base_url . $main_image; ?>
                    <h2 style="font-family:Arial; color:#336699; font-size:20px"><?php print $title; ?></h2>

                    <p style="font-family:Arial,Helvetica,sans-serif; font-size:13px; line-height:18px;">
					  <?php print $main_text; ?>
					</p>
                  </td>
                  <td valign="top" width="190" class="sidebar" align="left">
				    <table width="190" border="0" cellspacing="0" cellpadding="0">
					<tbody>
					<tr>
					<td width="border="0" cellpadding="0" style="height:1px; width:190px; padding:0; line-height:0;">
               <img src="<?php print $image_folder; ?>/vidste-du-top.gif"
            alt="Header" width="190" height="106" style="display:block"/> 
			</td>
			</tr>
			<tr>
			<td>
			  <p style="font-family:Arial,Helvetica,sans-serif; font-style:italic; margin-left:10px; margin-top:0; margin-right:10px; margin-bottom:0;  padding:0; padding-left:10px; padding-top: 10px; font-size:11px; padding-right:10px; background:#ff9700; color:#ffffff; height:84px;">
			"<?php print $vidst_du_text ?>"
			</p>
			</td>
			</tr>
			<tr>
			<td valign="top" height="12" width="190" style="padding:0; margin:0;">
			<img src="<?php print $image_folder; ?>/vidste-du-bottom.gif"
            alt="Header" width="190" height="12" style="display:block; padding:0; margin:0;"/>
			</td>
			</tr>
			</table>
                  </td>
                </tr>
                </tbody>
              </table>
            </td>
          </tr>

          <tr>
                <td colspan="3" width="550" style="padding-left:10px">
                 <img src="img/sep.gif"
            width="530" height="2" />
                </td>
                </tr>
                <tr>
                <td colspan="3" height="50" align="right" style="padding-right:10px;">
                	<a href="http://metropol-danmark.dk" title="Metropol-Danmark" style="color:#ff7900; font-family:Arial; font-size:11px; font-weight:bold; text-decoration:none;">Besog Metropol Danmark</a></td>
                </tr>
                
          <tr>
          
          	<td style="background-color:#414141;" colspan="2" height="40" width="550" valign="bottom">
            	<img src="<?php print $image_folder ?>/footer-top.gif" width="550" height="40" />
            </td>
          </tr>
            <td colspan="2" valign="top" align="left" style="background-color:#414141;" height="61">
              <p style="margin-left:20px; color:#949494; font-family:Arial; font-size:10px;">Skottenborg 26<br />
				8800 Viborg<br />
				info@metropoldanmark.dk<br />
				+45 87 28 51 76<br />
                </p>

      
            </td>
          </tr>
           
        </table>
      </td>
    </tr>
  </table>