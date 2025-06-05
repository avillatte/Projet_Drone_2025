/**
 * @files vues.js
 * @version 1
 * @author Benjamin BANDOU
 * @date 05/06/2025
 */

/** Initialisation de l'application à la fin du chargement du DOM */
$(document).ready(function () {
    initApp();

    if ($('#historique-table').length) {
        chargerTableHistorique();
    }
});

/** Affiche une alerte Bootstrap temporaire
 * @param {string} message - Message à afficher
 * @param {string} [type=success] - Type d'alerte : success, danger, warning, etc.
 */
function showAlert(message, type = 'success') {
    const alertId = `alert-${Date.now()}`;
    const alertHtml = `
        <div id="${alertId}" class="alert alert-${type} alert-dismissible fade show" role="alert">
            ${message}
            <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Fermer"></button>
        </div>
    `;
    $('#alert-container').append(alertHtml);

    setTimeout(() => {
        $(`#${alertId}`).alert('close');
    }, 5000);
}

/** Échappe les caractères HTML dans une chaîne de texte
 * @param {string} text
 * @returns {jQuery} - Chaîne échappée
 */
function escapeHtml(text) {
    return $('<div>').text(text).html();
}

/** Initialise les composants de l'application */
function initApp() {
    loadUserTable();
    loadClassesForSelect();
    initAddClasse();
    initAddUtilisateur();
    initEditUtilisateur();
    initDeleteUtilisateur();
}

/** Charge et affiche le tableau des utilisateurs */
function loadUserTable() {
    const basePath = window.location.href.substring(0, window.location.href.lastIndexOf("/") + 1);
    const urlJsonFr = basePath + "../libs/datatables/fr-FR.json";

    $.ajax({
        url: '../Controleurs/controleur_utilisateur.php',
        method: 'GET',
        data: { action: 'getUsers' },
        dataType: 'json',
        success: function (users) {
            if ($.fn.DataTable.isDataTable('#user-table')) {
                $('#user-table').DataTable().clear().destroy();
            }

            $('#user-table').DataTable({
                data: users,
                columns: [
                    { data: 'id_utilisateur', title: 'ID' },
                    { data: 'nom', title: 'Nom' },
                    { data: 'prenom', title: 'Prénom' },
                    { data: 'nom_classe', title: 'Classe' },
                    {
                        data: null,
                        title: 'Actions',
                        orderable: false,
                        searchable: false,
                        render: function (data, type, row) {
                            return `
                                <i class="bi bi-pencil-square mod" title="Modifier" style="cursor:pointer;" data-id="${row.id_utilisateur}"></i>
                                &nbsp;&nbsp;
                                <i class="bi bi-trash supp" title="Supprimer" style="cursor:pointer;" data-id="${row.id_utilisateur}"></i>
                            `;
                        }
                    }
                ],
                language: {
                    url: urlJsonFr
                },
                drawCallback: bindTableActions
            });
        },
        error: showAjaxError
    });
}


/** Charge les classes dans les listes déroulantes */
function loadClassesForSelect() {
    $.ajax({
        url: '../Controleurs/controleur_utilisateur.php',
        method: 'GET',
        data: { action: 'getClasses' },
        dataType: 'json',
        success: function (classes) {
            let options = '<option value="">-- Sélectionnez une classe --</option>';
            classes.forEach(c => {
                options += `<option value="${c.id_classes}">${escapeHtml(c.nom_classe)}</option>`;
            });
            $('#classeSelect, #editClasse').html(options);
        },
        error: showAjaxError
    });
}

/** Initialise le bouton d'ajout de classe */
function initAddClasse() {
    $('#ajout').on('click', function (e) {
        e.preventDefault();
        const classe = $('#classeAdd').val().trim();
        if (!classe) {
            showAlert('Veuillez saisir une classe.', 'warning');
            return;
        }

        $.ajax({
            url: '../Controleurs/controleur_utilisateur.php?action=ajouterClasse',
            method: 'POST',
            data: { classe },
            dataType: 'json',
            success: function (response) {
                if (response.success) {
                    showAlert('Classe ajoutée avec succès.');
                    $('#addClasseModal').modal('hide');
                    $('#classeAdd').val('');
                    loadClassesForSelect();
                } else {
                    showAlert('Erreur : ' + (response.message || 'Inconnue.'), 'danger');
                }
            },
            error: showAjaxError
        });
    });
}


/** Initialise le bouton d'ajout d'utilisateur */
function initAddUtilisateur() {
    $('#ajoutUtilisateur').on('click', function (e) {
        e.preventDefault();

        const nom = $('#nomAdd').val().trim();
        const prenom = $('#prenomAdd').val().trim();
        const id_classes = $('#classeSelect').val();

        if (!nom || !prenom || !id_classes) {
            showAlert('Veuillez remplir tous les champs.', 'warning');
            return;
        }

        $.ajax({
            url: '../Controleurs/controleur_utilisateur.php?action=ajouterUtilisateur',
            method: 'POST',
            data: { nom, prenom, id_classes },
            dataType: 'json',
            success: function (response) {
                if (response.success) {
                    showAlert('Utilisateur ajouté avec succès.');
                    $('#addUserModal').modal('hide');
                    $('#addUserForm')[0].reset();
                    loadUserTable();
                } else {
                    showAlert('Erreur : ' + (response.message || 'Inconnue.'), 'danger');
                }
            },
            error: showAjaxError
        });
    });
}


/** Gère les actions de modification et suppression sur le tableau */
function bindTableActions() {
    $('.mod').off('click').on('click', function () {
        const tr = $(this).closest('tr');
        const id = tr.data('id');
        const nom = tr.children('td').eq(1).text();
        const prenom = tr.children('td').eq(2).text();
        const classeNom = tr.children('td').eq(3).text();

        $('#editUserId').val(id);
        $('#editNom').val(nom);
        $('#editPrenom').val(prenom);

        $('#editClasse option').each(function () {
            if ($(this).text() === classeNom) {
                $(this).prop('selected', true);
            }
        });

        $('#editUserModal').modal('show');
    });

    $('.supp').off('click').on('click', function () {
        const tr = $(this).closest('tr');
        const id = tr.data('id');
        $('#deleteConfirmModal').data('id', id).modal('show');
    });
}


/** Initialise la modification d'un utilisateur */
function initEditUtilisateur() {
    $('#modifierUtilisateur').on('click', function (e) {
        e.preventDefault();

        const id = $('#editUserId').val();
        const nom = $('#editNom').val().trim();
        const prenom = $('#editPrenom').val().trim();
        const id_classes = $('#editClasse').val();

        if (!nom || !prenom || !id_classes) {
            showAlert('Veuillez remplir tous les champs.', 'warning');
            return;
        }

        $.ajax({
            url: '../Controleurs/controleur_utilisateur.php?action=modifierUtilisateur',
            method: 'POST',
            data: { id_utilisateur: id, nom, prenom, id_classes },
            dataType: 'json',
            success: function (response) {
                if (response.success) {
                    showAlert('Utilisateur modifié avec succès.');
                    $('#editUserModal').modal('hide');
                    loadUserTable();
                } else {
                    showAlert('Erreur : ' + (response.message || 'Inconnue.'), 'danger');
                }
            },
            error: showAjaxError
        });
    });
}

/** Initialise la suppression d'un utilisateur */
function initDeleteUtilisateur() {
    $('#confirmDeleteBtn').on('click', function () {
        const id = $('#deleteConfirmModal').data('id');

        $.ajax({
            url: '../Controleurs/controleur_utilisateur.php?action=supprimerUtilisateur',
            method: 'POST',
            data: { id_utilisateur: id },
            dataType: 'json',
            success: function (response) {
                if (response.success) {
                    if (response.rowCount > 0) {
                        showAlert('Utilisateur supprimé avec succès.');
                    } else {
                        showAlert('Aucun utilisateur trouvé avec cet ID.', 'warning');
                    }
                    $('#deleteConfirmModal').modal('hide');
                    loadUserTable();
                } else {
                    showAlert('Erreur serveur : ' + (response.error || 'inconnue.'), 'danger');
                }
            },
            error: showAjaxError
        });
    });
}


/** Charge et affiche le tableau de l'historique des simulations */
function chargerTableHistorique() {
    const basePath = window.location.href.substring(0, window.location.href.lastIndexOf("/") + 1);
    const urlJsonFr = basePath + "../libs/datatables/fr-FR.json";

    $.ajax({
        url: '../Controleurs/controleur_utilisateur.php',
        method: 'GET',
        data: { action: 'getHistorique' },
        dataType: 'json',
        success: function (data) {
            if (data.error) {
                window.location.href = 'connexion.php';
            } else {
                if ($.fn.DataTable.isDataTable('#historique-table')) {
                    $('#historique-table').DataTable().clear().destroy();
                }

                $('#historique-table').DataTable({
                    data: data,
                    columns: [
                        { data: 'id_simulation', title: 'ID' },
                        { data: 'date', title: 'Date' },
                        { data: 'utilisateur', title: 'Utilisateur' },
                        { data: 'classe', title: 'Classe' },
                        { data: 'scenario', title: 'Scénario' },
                        { data: 'objectif', title: 'Objectif' },
                        { data: 'drone', title: 'Drone' }
                    ],
                    language: {
                        url: urlJsonFr
                    },
                    paging: true,
                    ordering: true,
                    searching: true
                });
            }
        },
        error: function () {
            alert("Erreur lors du chargement de l'historique.");
        }
    });
}

/** Affiche une erreur AJAX dans une alerte
 * @param {jqXHR} xhr
 * @param {string} status
 * @param {string} error
 */
function showAjaxError(xhr, status, error) {
    showAlert(`Erreur de communication : ${error}`, 'danger');
}
