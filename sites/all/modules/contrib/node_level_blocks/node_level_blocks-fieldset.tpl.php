<?php
// $Id: node_level_blocks-fieldset.tpl.php,v 1.1 2010/05/19 19:54:26 skilip Exp $

/**
 * @file
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $block_regions: An array of regions. Keyed by name with the title as value.
 * - $block_listing: An array of blocks keyed by region and then delta.
 * - $form_closure: All not yet rendered form elements.
 *
 * Each $block_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $block_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 *
 * @see template_preprocess_node_level_blocks_fieldset()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'block') .'/block.js');
  foreach ($block_regions as $region => $title) {
    drupal_add_tabledrag('blocks', 'match', 'sibling', 'block-region-select', 'block-region-'. $region, NULL, FALSE);
    drupal_add_tabledrag('blocks', 'order', 'sibling', 'block-weight', 'block-weight-'. $region);
  }
?>
<table id="blocks" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Block'); ?></th>
      <th><?php print t('Region'); ?></th>
      <th><?php print t('Weight'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($block_regions as $region => $title): ?>
      <tr class="region region-<?php print $region?>">
        <td colspan="5" class="region"><?php print $title; ?></td>
      </tr>
      <tr class="region-message region-<?php print $region?>-message <?php print empty($block_listing[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="5"><em><?php print t('No blocks in this region'); ?></em></td>
      </tr>
      <?php foreach ($block_listing[$region] as $delta => $data): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?><?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
        <td class="block"><?php print $data->block_title; ?></td>
        <td><?php print $data->region_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php
print $form_submit;
